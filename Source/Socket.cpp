#include "Socket.h"

Socket::Socket() {
	hostname=IPADDRESS;
	port=PORT;
}

Socket::Socket(string hn, int pt) {
	hostname=hn;
	port=pt;
}

void Socket::connectar(void) {
	//http://msdn.microsoft.com/en-us/library/ms742213%28VS.85%29.aspx
	wVersion = MAKEWORD(2,2);	
	if (WSAStartup(wVersion, &wsaData) != 0) {
		WSACleanup();
		throw Exception("WSA Error");
	}

	//connectar
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		WSACleanup();
		throw Exception("Socket Error");
	}

	//receber host
	server.sin_family = AF_INET;
	ht = gethostbyname(hostname.c_str());
	if (ht == NULL)
		throw Exception("Host Error");

	//ligar-se a porta
	memcpy((char *)&server.sin_addr, (char *)ht->h_addr, ht->h_length);
	server.sin_port = htons(port);
	if (connect(sock, (struct sockaddr *)&server, sizeof server) == SOCKET_ERROR)
		throw Exception("Port Error");

}

string Socket::enviar(string msg) {
	
	/*int expected=10;
	if (msg.find("inicializaPosTorres")==0) {
		cout << "inicializaPosTorres" << endl;
		expected=344;
	} else if (msg.find("valida")==0) {
		cout << "valida" << endl;
		expected=0;
	} else if (msg.find("execute")==0) {
		cout << "execute" << endl;
		expected=0;
	} else if (msg.find("calculate")==0) {
		cout << "calculate" << endl;
		expected=352;
	} else if (msg.find("game_end")==0) {
		cout << "game_end" << endl;
		expected=8;
	} else if (msg.find("blocked")==0) {
		cout << "blocked" << endl;
		expected=12;
	} else {
		cout << "erro" << endl;
		return "erro";
	}*/
	
	//enviar mensagem
	if(send (sock, msg.c_str(), msg.size()+1, 0) == SOCKET_ERROR)
		throw Exception("Erro a enviar");

	cout << "enviou: " << msg << endl;
	if (msg.compare("bye")==0) return "ok";

	char recv_buf[1024]; 
	string recebe;
	memset(recv_buf,'\0',sizeof(recv_buf));

	//receber o resultado
	int recv_valor=0;
	//do {
	recv_valor = recv(sock, recv_buf, 1024, 0);
	//cout << "recebeu: " << recv_buf << " : " << recv_valor << " : " << expected << endl;
	//} while (recv_valor!=expected);
	recebe.assign(recv_buf,recv_valor-2);
	return recebe;
}

void Socket::desligar(void) {
	string ok = enviar("bye");
	shutdown(sock,SD_BOTH);
	closesocket(sock);
}

Tabuleiro::Tabuleiro() {

	tabuleiro.clear();

	jogador=1;
	linha=1;
	coluna=1;
	ntorre=1;
	pc_joga=0;

	modo=1;
	nivel=1;

	pt1=0;
	pt2=0;
	
}

void Tabuleiro::reset() {
	tabuleiro.clear();

	jogador=1;
	linha=1;
	coluna=1;
	ntorre=1;
	pc_joga=0;

	modo=1;
	nivel=1;
}

void Tabuleiro::printinfo() {
	cout << endl;
	cout << "jogador: " << jogador << endl;
	cout << "linha: " << linha << endl;
	cout << "coluna: " << coluna << endl;
	cout << "ntorre: " << ntorre << endl;
	cout << "pc_joga: " << pc_joga << endl;
	cout << "modo: " << coluna << endl;
	cout << "nivel: " << ntorre << endl;
	cout << "cena: " << pc_joga << endl;
}

void Tabuleiro::print() {
	cout << endl << "Tabuleiro: " << endl;
	for (unsigned int i = 0; i < tabuleiro.size(); i++)
	{
		for (unsigned int j = 0; j < tabuleiro[i].size(); j++)
		{
			cout << tabuleiro[i][j] << "  ";
		}
		cout << endl;
	}
}

void Tabuleiro::printall() {
	print();
	printinfo();
}

Parser::Parser() {
	parsesocket=new Socket();
	tabuleiro=new Tabuleiro();
}

Parser::Parser(Socket *psocket)
{
	parsesocket=psocket;
	tabuleiro=new Tabuleiro();
}

Parser::Parser(Socket *psocket, Tabuleiro *tab)
{
	parsesocket=psocket;
	tabuleiro=tab;
}

void Parser::inicia()
{
	string envia_prolog = "inicializaPosTorres(" + toString(1) + ").";
	string recebe_prolog;

	//do {
		recebe_prolog.clear();
		recebe_prolog = parsesocket->enviar(envia_prolog);
	//} while(recebe_prolog.compare("erro")==0);

	recebe_prolog.erase(0,4);
	tabuleiro->tabuleiro.clear();
	recebe_tabuleiro(recebe_prolog);
	recebe_prolog.erase(0,3);
}

void Parser::recebe_tabuleiro(string &recebe_str)
{
	//limpa o tabuleiro
	tabuleiro->tabuleiro.clear();
	//cout << "string recebida: " << recebe_str << endl;
	while(true)
	{
		//encontra a primeira linha
		int delta = recebe_str.find_first_of("]");
		if (delta<0) break;
		string array = recebe_str.substr(1,delta-1);
		//criamos um vector que representa uma linha
		vector<string> line;
		char *token = NULL; char *proximo = NULL;
		//encontra-se o primeiro elemento dessa linha
		token = strtok_s((char*) array.c_str(),",", &proximo);
		while (token != NULL){
			//encontramos os proximos elementos dessa linha divididos por ","
			//e adicionamos ao vector de elementos
			string str=token;
			line.push_back(str);
			token = strtok_s(NULL, ",", &proximo);
		}
		//adicionar essa linha completa ao tabuleiro
		tabuleiro->tabuleiro.push_back(line);
		//apagar a linha por onde ja passou o parser
		recebe_str.erase(0,delta+1); 
		if (!recebe_str.compare(0,1,","))
			recebe_str.erase(0,1); 
		else if (!recebe_str.compare(0,1,"]"))
			break;
	}
}

void Parser::envia_tabuleiro(string &envia_str)
{
	for (unsigned int i = 0; i < tabuleiro->tabuleiro.size(); i++)
	{
		//adicionar o parentesis em cada linha
		envia_str += "[";
		for (unsigned int j = 0; j < tabuleiro->tabuleiro[i].size(); j++)
		{
			envia_str += toString(tabuleiro->tabuleiro[i][j]);
			//adicionar virgulas entre cada elemento de uma linha,excepto se for o ultimo elemento
			if(j < (tabuleiro->tabuleiro[i].size()-1))
				envia_str += ",";
		}
		//fechar parentesis no final de cada linha
		envia_str += "]";
		//adicionar a virgula apos cada linha, caso nao seja a ultima
		if(i < (tabuleiro->tabuleiro.size()-1))
			envia_str += ",";
	}
}

void Parser::jogada_humana(int linha, int coluna)
{
	string str_enviada = "execute([";
	envia_tabuleiro(str_enviada);
	str_enviada += "]," +  toString(tabuleiro->ntorre) + "," + 
	toString(linha) + "," + toString(coluna) +"," + 
	toString(tabuleiro->jogador)+").";

	string str_prolog;
	//do {
		str_prolog.clear();
		str_prolog = parsesocket->enviar(str_enviada);
	//}while(str_prolog.size()<10);

	str_prolog.erase(0,4); 
	tabuleiro->tabuleiro.clear();
	recebe_tabuleiro(str_prolog);
	str_prolog.erase(0,2);

	int delta = str_prolog.find_first_of(",");
	string recv_jogador = str_prolog.substr(0,delta);
	tabuleiro->jogador = Integer(recv_jogador);
	str_prolog.erase(0,2); 

	delta = str_prolog.find_first_of(")");
	string recv_torre = str_prolog.substr(0,delta);
	tabuleiro->ntorre = Integer(recv_torre);
	str_prolog.erase(0,3); 

	tabuleiro->linha = linha;
	tabuleiro->coluna = coluna;
	tabuleiro->nivel=2;
}

void Parser::jogada_pc()
{
	string resultado = "calculate([";
	envia_tabuleiro(resultado);
	resultado += "]," + toString(tabuleiro->nivel) + "," + 
	toString(tabuleiro->jogador) + "," + toString(tabuleiro->ntorre) + ").";

	string str_prolog;
	//do {
		str_prolog.clear();
		str_prolog = parsesocket->enviar(resultado);
	//} while(str_prolog.size()<50);

	str_prolog.erase(0,4); 
	tabuleiro->tabuleiro.clear();
	recebe_tabuleiro(str_prolog);
	str_prolog.erase(0,2);

	int delta = str_prolog.find_first_of(",");
	tabuleiro->jogador = Integer(str_prolog.substr(0,delta));
	str_prolog.erase(0,2); 

	delta = str_prolog.find_first_of(",");
	string recv_ntorre = str_prolog.substr(0,delta);
	tabuleiro->ntorre = Integer(recv_ntorre);
	str_prolog.erase(0,2); 

	delta = str_prolog.find_first_of(",");
	string newcol = str_prolog.substr(0,delta);
	tabuleiro->coluna = Integer(newcol);
	str_prolog.erase(0,2); 

	delta = str_prolog.find_first_of(")");
	string newlin = str_prolog.substr(0,delta);
	tabuleiro->linha = Integer(newlin);
	str_prolog.erase(0,3); 
}

bool Parser::valida_jogada(int linha,int coluna)
{
	string resultado = "valida([";
	envia_tabuleiro(resultado);
	resultado += "]," + toString(tabuleiro->ntorre) + "," + toString(linha)+ "," + 
	toString(coluna) + "," + toString(tabuleiro->jogador) + ").";

	string str_prolog = parsesocket->enviar(resultado);
	//while(str_prolog.compare(0,3,"ok("))
	//	str_prolog = parsesocket->enviar(resultado);

	str_prolog.erase(0,3); 
	int delta = str_prolog.find_first_of(")");
	int validacao = Integer(str_prolog.substr(0,delta));

	return validacao;
}

bool Parser::deadlock(){
	string resultado = "blocked([";
	envia_tabuleiro(resultado);
	resultado += "],"+toString(tabuleiro->jogador)+","+toString(tabuleiro->ntorre)+").";

	string str_prolog;
	//do {
		str_prolog.clear();
		str_prolog = parsesocket->enviar(resultado);
	//} while(str_prolog.size()> 10);

	str_prolog.erase(0,3); 
	int delta = str_prolog.find_first_of(",");
	int presa = Integer(str_prolog.substr(0,delta));

	if(presa==0){	
		return false;
	}

	str_prolog.erase(0,2); 
	delta=str_prolog.find_first_of(",");
	tabuleiro->jogador=Integer(str_prolog.substr(0,delta));
	str_prolog.erase(0,2); 
	delta=str_prolog.find_first_of(")");
	tabuleiro->ntorre=Integer(str_prolog.substr(0,delta));

	return true;
}

int Parser::ganhou()
{
	string resultado = "game_end([";
	envia_tabuleiro(resultado);
	resultado += "]).";

	string str_prolog;
	//do {
		str_prolog.clear();
		str_prolog = parsesocket->enviar(resultado);
	//} while(str_prolog.size()>6);

	str_prolog.erase(0,3); 
	int delta = str_prolog.find_first_of(")");
	int vitoria = Integer(str_prolog.substr(0,delta));

	return vitoria;
}

/*
Jogo::Jogo() {}

void Jogo::jogada(Tabuleiro tab){
	jogadas.push(tab);
	filme.push(tab);
}

void Jogo::undo(Tabuleiro &tab){
	if(!jogadas.empty()){
		tab.coluna=jogadas.top().coluna;
		tab.jogador=jogadas.top().jogador;
		tab.linha=jogadas.top().linha;
		tab.modo=jogadas.top().modo;
		tab.nivel=jogadas.top().nivel;
		tab.ntorre=jogadas.top().ntorre;
		tab.pc_joga=jogadas.top().pc_joga;
		for (int i = 0; i < jogadas.top().tabuleiro.size(); i++)
			for (int j = 0; j < jogadas.top().tabuleiro[i].size(); j++)
					tab.tabuleiro[i][j] = jogadas.top().tabuleiro[i][j];
		filme.push(tab);
		jogadas.pop();
	}
}

void Jogo::apagar_jogadas(){
	while(!jogadas.empty())
		jogadas.pop();
}

int Jogo::fim_jogo(Tabuleiro &tab){

		int i=0;
		for (int j = 0; j < tab.tabuleiro[i].size(); j++)
		{
			string str= tab.tabuleiro[i][j];
			cout <<"jogador 1 "<< str<<endl;
			if(str.compare("'Y1'")==0 || str.compare("'Y2'")==0 || str.compare("'Y3'")==0 || str.compare("'Y4'")==0  || str.compare("'Y5'")==0 || str.compare("'Y6'")==0 || str.compare("'Y7'")==0 || str.compare("'Y8'")==0)
				return 2;
		}

		i=7;
		for (int j = 0; j < tab.tabuleiro[i].size(); j++)
		{

				string str= tab.tabuleiro[i][j];
				cout <<"jogador 2 "<< str<<endl;
			if(str.compare("'X1'")==0 || str.compare("'X2'")==0 || str.compare("'X3'")==0 || str.compare("'X4'")==0  || str.compare("'X5'")==0 || str.compare("'X6'")==0 || str.compare("'X7'")==0 || str.compare("'X8'")==0)
				return 1;
		}

	return 0;
}
*/