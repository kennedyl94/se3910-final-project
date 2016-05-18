server::server(int port)
{
    int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     int index;



     // Create a socket.
     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     // If the return is less than 0, then the socket failed to create.
     if (sockfd < 0)
     {
        error("ERROR opening socket");
     }

     // Initialize the buffer to all zeros.
     memset((void*) &serv_addr, 0, sizeof(serv_addr));

     // Obtain the port number as an integer.
     portno = port;

     // Setup the server address structure.
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     // Bind the socket appropriately.
     if (bind(sockfd, (struct sockaddr *) &serv_addr,   sizeof(serv_addr)) < 0)
     {
              error("ERROR on binding");
     }
     // Listen on the socket for an incoming connection.  The parameter is the number of connections that can be waiting / queued up.  5 is the maximum allowed by most systems.
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

     // Block until a client has connected to the server.  This returns a file descriptor for the connection.
    newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
    // If the return is less than 0l, there is an error.
     if (newsockfd < 0)
       {
          error("ERROR on accept");
       }
     // Fill the buffer with all zeros.
     memset(&buffer[0], 0, sizeof(buffer));



}




int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    server s(1234);


    return 0;
}

