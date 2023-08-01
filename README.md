# Poker game project

Spectacular and interactive Texas Hold'em game that supports multiple tables, players and viewers simultaneously. full implementation of TCP network protocol, sha256 passwords encryption, advanced multithreading wait & synchronization and High-level SFML graphics design.


Requirements: 
* gcc compiler version 11.3.0 and above.
* linux (ubuntu) virtualization (GUI) enable.
* Installing SFML library - https://www.sfml-dev.org/tutorials/2.5/start-linux.php.



1. Run the server:   
    a. set server ip & port:

          go to file - Poker-game-project/server/src/resources/server_ip_and_port.txt 
          Write in first line - server ip.
          Write in second line - server port.   
    b. bild & run server:

          go to directory - Poker-game-project/server/src
          run command - make check.
         
2. Run the client:   
    a. set server ip & port: 

          go to file - Poker-game-project/client/src/resources/txt/server_ip_and_port.txt  
          Write in first line - server ip.
          Write in second line - server port. 

    b. bild & run client:
    
          go to directory - Poker-game-project/client/src
          run command - make check.

## License
This project is licensed under the Attribution-NonCommercial 4.0 International (CC BY-NC 4.0) - see the LICENSE file for details
