----------README----------
-COMO CORRER-
Antes de correr o execut�vel deve consultar em SICStus o ficheiro kamisado.pl e chamar o predicado server.
Aconselha-se a resolu��o m�nima de 1280x800 em janela maximizada.
-JOGO E REGRAS-
O jogo � o Kamisado e obedece �s regras por defeito do jogo original.
As primeiras pe�as a jogar s�o as pretas. O jogador tem de escolher uma pe�a no inicio e de seguida uma casa.
A cor da casa que escolher vai definir qual � a pe�a que o pr�ximo jogador vai jogar.
O objectivo � fazer uma pe�a de um jogador chegar ao outro lado do tabuleiro, ou seja, �s casas mais afastadas.
-MODOS E N�VEIS-
O jogo apresenta 3 modos de jogo que est�o previstos no programa realizado em PLOG:
Humano VS Humano, Humano VS PC, PC VS PC, e 2 n�veis: F�cil e Dif�cil, para os modos com PC.
A troca de modos e n�veis pode ser feita em tempo real sem a necessidade de reiniciar o jogo,
ou seja, o programa permite que, por exemplo, se estiver em modo Humano VS Humano e quiser que o outro jogador passe a ser controlado por computador, pode faze-lo ao seleccionar a op��o Humano VS PC. E se achar que a I.A. est� f�cil demais, pode alterar para modo dif�cil ou vice-versa.
Todas estas possibilidades foram feitas a pensar num maior dinamismo e aumento do leque de op��es de jogo.
-CONTADOR E REL�GIO-
Por defeito, um jogador tem 60 segundos para jogar. Isto pode ser alterado atrav�s de um "spinner" no menu.
A contagem do tempo � feita por um rel�gio que agrupa os segundos em cada 10.
A ideia veio de um contador de contadores que, ao agrupar os tempos, d� uma sensa��o de maior press�o no jogador.
Poder� notar que a cada dez segundos � incrementado um contador de um d�gito.
Isto significa