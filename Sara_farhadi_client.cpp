//Sara Farhadi client program
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/wait.h>
#include <algorithm>
#include <string>

using namespace std;
void error(char *msg)
{
	perror(msg);
	exit(0);
}
struct message 
{
	int destination;
	int value;
};
struct newMgs {
	int em[12];
	int wc[4];
};
int main(int argc, char *argv[])
{
	int sockfd;
	int portno, n, i, j, y, p, pid, counter;
	char buffer[256];
	struct sockaddr_in serv_addr;
	struct hostent *server;
	message data[3];
	newMgs newData[3];
	int em1[4], em2[4], em3[4], dm1[4], dm2[4], dm3[4], m1, m2, m3, m[3], val;
	if (argc < 3) {
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	/*sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd[i] < 0)
		error("ERROR opening socket");*/
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}
	counter = 0;
	for (i = 0; i < 3; i++) 
	{
		counter++;
		cin >> data[i].destination >> data[i].value;
		if ((pid = fork()) == 0)
		{
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if (sockfd < 0) {
				error("ERROR opening socket");
			}
			bzero((char *)&serv_addr, sizeof(serv_addr));
			serv_addr.sin_family = AF_INET;
			bcopy((char *)server->h_addr,
				(char *)&serv_addr.sin_addr.s_addr,
				server->h_length);
			serv_addr.sin_port = htons(portno);
			
			connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
			/*if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
				error("ERROR connecting");*/

			cout << "Child " << counter << ", sending value: " << data[i].value << " to child process ";
			cout << data[i].destination << endl;

			write(sockfd, &data[i], sizeof(struct message));
			/*n = write(sockfd, &data[i], sizeof(struct message));
			if (n < 0) {
				error("ERROR writing to socket");
			}*/

			read(sockfd, &newData[i], sizeof(struct newMgs));
			/*n = read(sockfd, &newData[i], sizeof(struct newMgs));
			if (n < 0) {
				error("ERROR reading from socket");
			}*/
			cout << endl;
			cout << "Child " << counter << endl;
			cout << "Signal: ";
			for (j = 0; j < 12; j++) {
				cout << newData[i].em[j];
			}
			cout << endl;
			cout << "Code: ";
			for (j = 0; j < 4; j++) {
				cout << newData[i].wc[j];
			}
			cout << endl;
			memcpy(em1, newData[i].em, 4 * sizeof(int));
			memcpy(em2, &newData[i].em[4], 4 * sizeof(int));
			memcpy(em3, &newData[i].em[8], 4 * sizeof(int));
			for (y = 0; y < 4; y++) {
				dm1[y] = em1[y] * newData[i].wc[y];
				dm2[y] = em2[y] * newData[i].wc[y];
				dm3[y] = em3[y] * newData[i].wc[y];
			}
			m1 = 0; m2 = 0; m3 = 0;
			for (y = 0; y < 4; y++) {
				m1 += dm1[y];
				m2 += dm2[y];
				m3 += dm3[y];
			}
			m[0] = m1 / 4;
			m[1] = m2 / 4;
			m[2] = m3 / 4;
			val = 0;
			for (p = 0; p < 3; p++) {
				if (m[p] == -1) {
					m[p] = 0;
				}
				val = val * 2 + m[p];
			}
			cout << "Received value = " << val << endl;

			_exit(0);
			//sleep(10);
			//break;
		}
	}
	for (int i = 0; i < 3; i++)
		wait(0);
	//close(sockfd);
	return 0;
}
