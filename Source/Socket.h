#ifndef SOCKET_H
#define SOCKET_H

//LIBRARIAS
#include <iostream>
#include <string>
#include <cstring>	
#include <sstream>
#include <cstdlib>
//#include <windows.h>
#include <winsock2.h>
#include <math.h>
#include <time.h>
#include <dos.h>
#include <stdio.h>
#include <conio.h>

#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>

#define IPADDRESS "127.0.0.1"
#define PORT 60070

using namespace std;

class Exception {
public:
	string erro; 
	Exception(string msg) {
		erro=msg;
	};
};

class Socket {
public:
	WORD wVersion;
	WSADATA wsaData;
	SOCKET sock;

	struct sockaddr_in server;
	struct hostent *ht;
	string hostname;
	unsigned short int port;

	Socket();
	Socket(string hn, int pt);
	void connectar(void);
	string enviar(string msg);
	void desligar(void);
};

class Tabuleiro
{
public:
	vector<vector<string>> tabuleiro;

	//ultima jogada
	int jogador;
	int linha;	
	int coluna;
	int ntorre;
	int pc_joga;

	//definicoes
	int modo;
	int nivel;

	//pontuacao
	int pt1;
	int pt2;

	Tabuleiro();
	void reset();
	void print();
	void printinfo();
	void printall();
};

class Parser
{
public:
	Socket *parsesocket;
	Tabuleiro *tabuleiro;

	Parser();
	Parser(Socket *psocket);
	Parser(Socket *psocket, Tabuleiro *tab);

	void inicia();
	void recebe_tabuleiro(string &recebe_str);
	void envia_tabuleiro(string &envia_str);

	void jogada_humana(int linha, int coluna);
	void jogada_pc();

	bool valida_jogada(int linha,int coluna);

	int ganhou();
	bool deadlock();
	
	template <class T> 
	static string toString(T val) {
		//performs an output operation on a stream 
		//generally involving some sort of formatting of the data
		stringstream ss;
		string str;
		ss << val;
		str = ss.str();
		return str;
	};

	static int Integer(string val) {
		//performs an input operation on a stream 
		//generally involving some sort of interpretation of the data
		stringstream ss(val);
		int numero;
		ss >> numero;
		return numero;
	};
};


/*
class Jogo
{
public:
	stack <Tabuleiro> jogadas;
	queue <Tabuleiro> filme;
	queue <Tabuleiro> filme_aux;
	int fim_jogo(Tabuleiro &tab);

	Jogo();
	void jogada(Tabuleiro tab);
	void apagar_jogadas();
	void undo(Tabuleiro &tab);
};*/

#endif
