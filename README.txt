----------README----------
-COMO CORRER-
Antes de correr o executável deve consultar em SICStus o ficheiro kamisado.pl e chamar o predicado server.
Aconselha-se a resolução mínima de 1280x800 em janela maximizada.
-JOGO E REGRAS-
O jogo é o Kamisado e obedece às regras por defeito do jogo original.
As primeiras peças a jogar são as pretas. O jogador tem de escolher uma peça no inicio e de seguida uma casa.
A cor da casa que escolher vai definir qual é a peça que o próximo jogador vai jogar.
O objectivo é fazer uma peça de um jogador chegar ao outro lado do tabuleiro, ou seja, às casas mais afastadas.
-MODOS E NÍVEIS-
O jogo apresenta 3 modos de jogo que estão previstos no programa realizado em PLOG:
Humano VS Humano, Humano VS PC, PC VS PC, e 2 níveis: Fácil e Difícil, para os modos com PC.
A troca de modos e níveis pode ser feita em tempo real sem a necessidade de reiniciar o jogo,
ou seja, o programa permite que, por exemplo, se estiver em modo Humano VS Humano e quiser que o outro jogador passe a ser controlado por computador, pode faze-lo ao seleccionar a opção Humano VS PC. E se achar que a I.A. está fácil demais, pode alterar para modo difícil ou vice-versa.
Todas estas possibilidades foram feitas a pensar num maior dinamismo e aumento do leque de opções de jogo.
-CONTADOR E RELÓGIO-
Por defeito, um jogador tem 60 segundos para jogar. Isto pode ser alterado através de um "spinner" no menu.
A contagem do tempo é feita por um relógio que agrupa os segundos em cada 10.
A ideia veio de um contador de contadores que, ao agrupar os tempos, dá uma sensação de maior pressão no jogador.
Poderá notar que a cada dez segundos é incrementado um contador de um dígito.
Isto significa