#include <future>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <optional>
#include <thread>

#include "display/DisplayTextManager.h"
#include "helper/Accumulator.h"
#include "GameManager.h"
#include "GraphicsManager.h"
#include "display/StaticGraphicsManager.h"
#include "helper/visual/FPS.h"
#include "pressure/DecryptJob.h"
#include "pressure/TowerPressureDecrypt.h"

#include "mpi.h" //
#include "pressure/job_spawners/MainJobSpawner.h"
#include "pressure/job_spawners/SecondaryJobSpawner.h"

void game_core() {
    GraphicsManager graphicsManager;
    const DisplayTextManager displayTextManager;
    const StaticGraphicsManager menuBackgroundManager;
    GameManager gameManager;
    FPS fps;

    const auto drawnPath = std::make_shared<sf::VertexArray>(sf::VertexArray());
    drawnPath->setPrimitiveType(sf::PrimitiveType::LineStrip);

    graphicsManager.addDrawables(gameManager.getAvailTowerDrawables());
    graphicsManager.addDrawables(gameManager.getDrawables());
    graphicsManager.addPriorityDrawable(drawnPath);
    graphicsManager.addLowPriorityDrawables(menuBackgroundManager.getStaticDrawables());
    graphicsManager.addPriorityDrawables(displayTextManager.getTextDrawables());

    setActiveCoresTo(3);

    while (graphicsManager.isActive()) {
        while (const std::optional event = graphicsManager.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                graphicsManager.deactivate();
                for (int i = 0; i < drawnPath->getVertexCount(); i++) {
                    printf("{{  %ff,   %ff}, sf::Color::Red, { 0.0f,  0.0f}}", drawnPath->operator[](i).position.x,
                           drawnPath->operator[](i).position.y);
                    if (i != drawnPath->getVertexCount() - 1) {
                        printf(",\n");
                    } else {
                        printf("\n");
                    }
                }
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                if (const auto buttonPressed = event->getIf<sf::Event::MouseButtonPressed>();
                    buttonPressed->button == sf::Mouse::Button::Left) {
                    const auto mousePosition = buttonPressed->position;
                    if (!gameManager.isTowerAlreadySelected()) {
                        if (gameManager.attemptSelectingTower(mousePosition)) {
                            graphicsManager.addPriorityDrawable(gameManager.getHoveredTowerDrawable());
                            graphicsManager.addPriorityDrawable(gameManager.getHoveredTower()->getRangeIndicator());
                        }
                    } else {
                        graphicsManager.removeDrawable(gameManager.getHoveredTowerDrawable());
                        graphicsManager.removeDrawable(gameManager.getHoveredTower()->getRangeIndicator());
                    }
                } else if (buttonPressed->button == sf::Mouse::Button::Right) {
                    gameManager.shrinkEnemyPath();
                }
            }

            if (event->is<sf::Event::KeyPressed>()) {
                if (const auto buttonPressed = event->getIf<sf::Event::KeyPressed>();
                    buttonPressed->code == sf::Keyboard::Key::Space) {
                    graphicsManager.setFramerateLimit(240);
                } else {
                    graphicsManager.setFramerateLimit(60);
                }
            }

            if (event->is<sf::Event::MouseButtonReleased>()) {
                if (const auto buttonReleased = event->getIf<sf::Event::MouseButtonReleased>();
                    buttonReleased->button == sf::Mouse::Button::Left) {
                    if (gameManager.isTowerAlreadySelected()) {
                        const auto success = gameManager.addTower(gameManager.getHoveredTower());
                        graphicsManager.removeDrawable(gameManager.getHoveredTower()->getRangeIndicator());
                        gameManager.deselectTower();
                        if (!success) {
                            graphicsManager.removeDrawable(gameManager.getHoveredTowerDrawable());
                        }
                    }
                }
            }
        }

        if (gameManager.isTowerAlreadySelected()) {
            const auto mousePosition = graphicsManager.getMousePosition();
            gameManager.dragSelectedTower(mousePosition);
        }

        gameManager.update();

        displayTextManager.setLifeCounterValue(gameManager.getPlayerHealth());
        displayTextManager.setWaveCounterValue(gameManager.getCurrentWaveNumber(), gameManager.getMaxWaveNumber());

        displayTextManager.setPlayerBalanceValue(gameManager.getPlayerBalance());

        graphicsManager.addDrawables(gameManager.getNewDrawables());
        graphicsManager.removeDrawables(gameManager.getRemovableDrawables());
        graphicsManager.draw();
        gameManager.cleanup();

        fps.update();
        displayTextManager.setFPSCounterValue(fps.getFPS());

        displayTextManager.setRemainingPressureValue(static_cast<int>(toDecrypt.size()) + currentOperations);
        displayTextManager.setActivePressureJobsValue(currentOperations);
        displayTextManager.setPressureCompletionRateValue(completionRate.getAverageRate());
        displayTextManager.setPressureProductionRateValue(additionRate.getAverageRate());
    }
}

//int main() {
//    game_core();
//}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size, name_len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(hostname, &name_len);

    // runs the SAME code on all processes with a unique rank
    // only capstone rank 0 can run game_core

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            nodeStateMap.insert({i, {0, 0}});
        }
        std::cout << "Main game running from: " << hostname << std::endl;
        std::thread consumerThread(priPressureConsumer);
        consumerThread.detach();
        std::cout << "Main game's consumer thread online. Node: " << hostname << std::endl;
        std::thread producerThread(priPressureProducer);
        producerThread.detach();
        std::cout << "Main game's producer thread online. Node: " << hostname << std::endl;
        game_core();
    } else {
        std::cout << "Pressure decrypter node running from: " << hostname << std::endl;
        std::thread consumerThread(secPressureConsumer);
        consumerThread.detach();
        std::cout << "Pressure decrypter node's consumer thread online. Node: " << hostname << std::endl;
        std::thread producerThread(secDecryptSpawner);
        std::cout << "Pressure decrypter node's producer thread online. Node: " << hostname << std::endl;
        producerThread.join();
    }

    MPI_Finalize();
    return 0;
}

/*
 *    if (rank == 0) {
        std::cout << "I Sleep.\n" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        std::cout << "Woke!!!!?\n\n" << std::endl;

        std::vector<std::string> received_messages;
        MPI_Status status;
        int alien_probe = 0;
        int node = 1;
        int i = 0;
        while (true) {
            // Adjust the number of messages as needed
            MPI_Iprobe(node, 1, MPI_COMM_WORLD, &alien_probe, &status);
            if (!alien_probe) {
            } else {
                // Probe to find out the incoming message size
                MPI_Probe(node, 1, MPI_COMM_WORLD, &status);

                int message_size;
                MPI_Get_count(&status, MPI_UNSIGNED_CHAR, &message_size);

                // Allocate just enough space
                std::vector<char> buffer(message_size);

                // Receive the message
                MPI_Recv(buffer.data(), message_size, MPI_UNSIGNED_CHAR, node, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                // Convert to std::string and store
                received_messages.emplace_back(buffer.data(), message_size - 1); // Exclude null terminator
                i++;
                if (i >= 14 && i < 30) {
                    node = 2;
                } else if (i >= 30) {
                    break;
                }
            }
        }

        // Print the received messages
        std::cout << "Received messages: \n";
        for (const auto &msg: received_messages) {
            std::cout << msg << std::endl;
        }
    } else if (rank == 1) {
        std::vector<std::string> messageTest = {
            "Hi", "We",
            "Are", "Testing", "To", "See", "if", "These", "messages",
            "Get", "Stored", "in a", "Queue.", "Wouldn't that be great?"
        };
        for (const auto &msg: messageTest) {
            MPI_Send(msg.c_str(), msg.size() + 1, MPI_UNSIGNED_CHAR, 0, 1, MPI_COMM_WORLD);
        }
    } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::vector<std::string> messageTest = {
            "Muddafucka", "I",
            "Am", "Node", "3", "n", "m", "boutta", "find",
            "where", "u", "live if  i", "don't.", "see my mes",
            "sages last. Capiche?", "XOXOXO"
        };
        for (const auto &msg: messageTest) {
            MPI_Send(msg.c_str(), msg.size() + 1, MPI_UNSIGNED_CHAR, 0, 1, MPI_COMM_WORLD);
        }
    }
    */