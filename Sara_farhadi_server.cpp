//Sara Farhadi
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <algorithm>
#include <string>
#include <math.h>
using namespace std;
void error(char *msg)
{
	perror(msg);
	exit(1);
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
	int newsockfd[3];
	int sockfd, portno, clilen, k, j, h, p, x, counter;
	struct sockaddr_in serv_addr, cli_addr;
	int n, i, num[3],  bin[3], bip[3];
	char buffer[256];
	message data[3];
	char in[3];
	int w1[4] = { -1, 1, -1, 1 };
	int w2[4] = { -1, -1, 1, 1 };
	int w3[4] = { -1, 1, 1, -1 };
	newMgs newData[3];
	int em[12], em1[12], em2[12], em3[12];
	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	bzero((char *)&serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	for (i = 0; i < 3; i++)
	{
		newsockfd[i] = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
		if (newsockfd[i] < 0)
			error("ERROR on accept");
		counter++;
		read(newsockfd[i], &data[i], sizeof(struct message));
		/*n = read(newsockfd[i], &data[i], sizeof(struct message));
		if (n < 0) error("ERROR reading from socket");*/

		cout << "Here is the message from child " << counter << ": Value = ";
		cout<< data[i].value << ", Destination = " << data[i].destination << endl;
	}
	
	for (i = 0; i < 3; i++) {
		int l = 0;
        int temp ;
        temp = data[i].value;
			while (temp >0)
			{
                
				num[l] = (temp % 2);
                temp = (temp / 2);
				l++;
                if(temp <2)
                    num[l]=0;
		}
		k = 0;
			for (j = 2; j >= 0 ; j--) {
				bin[k] = num[j];
				cout << bin[k];
					k++;
		}
		cout << endl;
		for (k = 0; k < 3; k++) {
			if (bin[k] == 0) {
				bip[k] = -1;
			}
			if (bin[k] == 1) {
				bip[k] = +1;
			}
		}
		p = 0;
		if (i == 0) {
			for (k = 0; k < 3; k++) {
				for (h = 0; h < 4; h++) {
					em1[p] = bip[k] * w1[h];
					p++;
				}
			}
		}
		if (i == 1) {
			for (k = 0; k < 3; k++) {
				for (h = 0; h < 4; h++) {
					em2[p] = bip[k] * w2[h];
					p++;
				}
			}
		}
		if (i == 2) {
			for (k = 0; k < 3; k++) {
				for (h = 0; h < 4; h++) {
					em3[p] = bip[k] * w3[h];
					p++;
				}
			}
		}
	}
	for (x = 0; x < 12; x++) {
		em[x] = em1[x] + em2[x] + em3[x];
		for (i = 0; i < 3; i++) {
			newData[i].em[x] = em[x];
		}
	}
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 4; j++) {
			if (i == 0) {
				newData[data[i].destination-1].wc[j] = w1[j];
				//cout << newData[data[i].destination - 1].wc[j];
			}
			if (i == 1) {
				newData[data[i].destination-1].wc[j] = w2[j];
				//cout << newData[data[i].destination - 1].wc[j];
			}
			if (i == 2) {
				newData[data[i].destination-1].wc[j] = w3[j];
				//cout << newData[data[i].destination - 1].wc[j];
			}
		}
		//cout << endl;
	}
	for (int i = 0; i < 3; i++) 
	{
	write(newsockfd[i], &newData[i], sizeof(struct newMgs));
		/*n = write(newsockfd[y], &newData[y], sizeof(struct newMgs));
		if (n < 0) error("ERROR writing to socket");*/
		sleep(2);
	}
	for (i = 0; i < 3; i++) {
		close(newsockfd[i]);
	}
	close(sockfd);
	return 0;
}
