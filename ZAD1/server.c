#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>		// inet_ntoa
#include <time.h> 			// time
//======================================================================
int error( int ern, const char *msg ){
    perror( msg );
    return ern;
}
//======================================================================
int main( void ) {
	
	/* zmienne predefiniowane *****************************************/
	int  port   = 12346;
	
	/* zmienne niezainicjowane ****************************************/
	int 		n;		
	int 		fd, newfd;
	char 		buffer[512];
	socklen_t 	socklen;
	
	struct 	sockaddr_in serv_addr, cli_addr;
	
	/* tworzymy gniazdo ***********************************************/
	fd = socket( AF_INET, SOCK_STREAM, 0 );		// tworzymy nowe gniazdo
	if (fd < 0) 
        return error( 1, "socket()" );
        
    /* zapelniamy strukture zerami (inicjujemy) ***********************/
	bzero( ( char * ) &serv_addr, sizeof( serv_addr ) );	// zapelniamy strukture zerami
	
	/* przypisujemy parametry *****************************************/
	serv_addr.sin_family 	  	= AF_INET;			// typ gniazda 
	serv_addr.sin_addr.s_addr 	= INADDR_ANY;		// oczekujemy polaczen na kazdym adresie
	serv_addr.sin_port 			= htons(port);		// port, na ktorym nasluchujemy
	
	/* mapujemy gniazdo ***********************************************/
	if( bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ) 
		return error( 2, "bind()" );
	
	/* rozpoczynamy nasluchiwanie na gniezdzie ************************/
	listen( fd, 5 );
	
	/* kod obslugujacy nowe polaczenia ********************************/
	socklen = sizeof(cli_addr);
	
	while( ( newfd = accept( fd, (struct sockaddr *) &cli_addr, &socklen ) ) ){
		/* sprawdzamy, czy udalo sie nawiazac polaczenie **************/
		if( newfd < 0 ){	// jesli wystapil blad, to wyswietlamy o nim informacje
			perror( "accept()" );
		}else{					// a jesli bledu nie bylo...
			/* wypisujemy informacje o polaczeniu *********************/
			printf("Adres: %s Port: %d\n",  inet_ntoa( cli_addr.sin_addr ), cli_addr.sin_port );
            
            char hex[512];
            bzero(buffer, 256);

			n = read(newfd, buffer, 255);

			if( n < 0 ) 
				perror("read()");
			
			int len = strlen(buffer);
            
			for (int i = 0, j = 0; i < len; ++i, j += 2)
			{
				sprintf(hex + j, "%02x", buffer[i] & 0xff);
			}

			printf("Twoja wiadomość w postaci hexadecymalnej: %s\n\n", hex);

			/* konczymy polaczenie ************************************/
			close(newfd);
		}
	}
	 /* zamykamy gniazdo **********************************************/
	close(fd);
	
	//------------------------------------------------------------------
	return 0; 
}
//======================================================================
