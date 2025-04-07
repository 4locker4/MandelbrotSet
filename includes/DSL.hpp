#ifndef DSL_HPP
#define DSL_HPP

#define MoveUp     sf::Keyboard::isKeyPressed (sf::Keyboard::Up)
#define MoveLeft   sf::Keyboard::isKeyPressed (sf::Keyboard::Left)
#define MoveDown   sf::Keyboard::isKeyPressed (sf::Keyboard::Down)
#define MoveRight  sf::Keyboard::isKeyPressed (sf::Keyboard::Right)

#define ScalePlus  sf::Keyboard::isKeyPressed (sf::Keyboard::Equal)
#define ScaleMinus sf::Keyboard::isKeyPressed (sf::Keyboard::Dash)

#define ExitWindow sf::Keyboard::isKeyPressed (sf::Keyboard::Q)

#define CheckToSaveCoords   if (sf::Keyboard::isKeyPressed (sf::Keyboard::LControl))        \
                                if (sf::Keyboard::isKeyPressed (sf::Keyboard::S))           \
                                {                                                           \
                                    ;                                                       \
                                }                                                           

#endif  // DSL_HPP