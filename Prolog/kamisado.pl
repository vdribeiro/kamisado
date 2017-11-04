 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%                        KAMISADO                         %%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
:-use_module(library(random)).
:-use_module(library(time)).
:-use_module(library(sockets)).
:- use_module(library(lists)).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%        INICIO JOGO        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
inicio:-  apresentacao,
          estado(Tab,PosTorres),
          write('Escolha modo de jogo:'),nl,
          write('1 - Jogador Vs Jogador'),nl,
          write('2-  Computador Vs Computador'),nl,
          write('3-  Jogador Vs Computador'),nl,
          %insiste(1, 3, Opcao),
          read(Opcao),
          ModoJogo is Opcao -1,
          pre_game(Tab,PosTorres,ModoJogo).

apresentacao:-
write(' ------Kamisado------ '),nl.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%            JOGO           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
pre_game(Tab,PosTorres,ModoJogo):-
                     ver_estado(Tab,PosTorres),((      ModoJogo=:=0,           %Jogador Vs Jogador
                            random(1,2,Jogador),
                            write('Jogador '),write(Jogador),
                            write(' Torre: '),read(Torre),
                            game(Jogador,Tab,PosTorres,ProximoJogador,Torre,ModoJogo,0)
                      )
                      ;
                      (      ModoJogo=:=1,          %PC Vs PC
                             random(1,2,Jogador),
                             random(1,8,Torre),
                             game(Jogador,Tab,PosTorres,ProximoJogador,Torre,ModoJogo,1)
                      )
                       ;
                      (      ModoJogo=:=2,          %Jogador Vs Pc
                             write('Nivel de jogo?'),nl,
                             write('1 - Facil'),nl,
                             write('2 - Medio'),nl,
                             read(Nivel),
                             write('Torre: '),read(Torre),
                             game(1,Tab,PosTorres,ProximoJogador,Torre,ModoJogo,Nivel)
                      )).


game(Jogador,Tab,PosTorres,ProximoJogador,CorTorre,ModoJogo,Nivel):-
                     repeat, (
                        (
                              (
                                   torre(Jogador,CorTorre,NomeTorreAux3),
                                   corCasa(Jogador,CorTorre,Tab,PosTorres,CorCasaAux2),
                                   proximoJogador(Jogador,ProximoJogadorAux),
                                   torre(ProximoJogadorAux,CorCasaAux2,NomeTorreAdversario),
                                   not(deadlock(Jogador,NomeTorreAux3,PosTorres)),
                                   not(deadlock(ProximoJogadorAux,NomeTorreAdversario,PosTorres)),
                                   write('Ambas as torres estao presas. O jogador '),write(Jogador),write(' ganhou, visto que o adversario provocou o deadlock.'),nl,
                                   inicio
                              )
                              ;  (
                                 torre(Jogador,CorTorre,NomeTorreAux),
                                 not(deadlock(Jogador,NomeTorreAux,PosTorres)),
                                 write(' A torre '),write(NomeTorreAux), write(' do jogador '),write(Jogador),
                                 write(' esta presa. Vai passar a vez ao adversario.'),nl,
                                 proximoJogador(Jogador,ProximoJogadorAux2),
                                 corCasa(Jogador,CorTorre,Tab,PosTorres,CorCasaAux),
                                 game(ProximoJogadorAux2,Tab,PosTorres,X,CorCasaAux,ModoJogo,Nivel)
                              )
                        )
                       ;
                       (     (
                             proximoJogador(Jogador,ProximoJogador),
                             torre(Jogador,CorTorre,NomeTorre),
                             (
                                ( ModoJogo=:=1,
                                         ((Jogador =:=1,pede_jogada(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,ModoJogo,1))
                                         ;
                                         pede_jogada(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,ModoJogo,2))
                                 );
                                ( ModoJogo=:=2,
                                         ((Jogador =:=1,pede_jogada(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,0,0))
                                         ;
                                         pede_jogada(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,1,Nivel))
                               );
                               pede_jogada(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,ModoJogo,Nivel)
                             ),
                             movimentar(Jogador,NomeTorre,ColWin,LinWin,Tab,PosTorres,NovoTabuleiro),
                             corCasa(Jogador,CorTorre,Tab,NovoTabuleiro,NewCorCasa)
                             ),
                             (
                                (Jogador =:=1,LinWin=:=8, write('O jogador '),  write(Jogador),write(' ganhou. Parabens!!!'),nl,nl,nl,inicio)
                                ;
                                (Jogador =:=2,LinWin=:=1, write('O jogador '), write(Jogador),write(' ganhou. Parabens!!!'),nl,nl,nl,inicio)
                                ;
                                 (
                                   %write('Prima qualquer tecla para continuar...'),nl,get_code(Tecla),
                                   game(ProximoJogador,Tab,NovoTabuleiro,X,NewCorCasa,ModoJogo,Nivel))
                             )
                       )
                    ).

pede_jogada(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,ModoJogo,Nivel):-
                      (write('Jogador: '),write(Jogador), write(' , vai jogar torre: '), write(NomeTorre),nl)
                  ,(
                      (      ModoJogo=:=0,
                             write('Linha:'),read(Linha),
                             write('Coluna:'),read(Coluna),nl,
                             membrotab(NomeTorre,Colini,Linhini,PosTorres),
                             valida_movimento(Jogador,Colini,Coluna,Linhini,Linha,PosTorres),
                             ColWin is Coluna, LinWin is Linha
                      )
                      ;
                      (      ModoJogo=:=1   ,(
                                   (         Nivel=:=1,
                                             gerar_mov(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin),
                                             write(' . IA de nivel 1 gerou ')
                                    )
                                    ;
                                    (         Nivel=:=2,
                                              gerar_greedy(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin),
                                              write(' . IA de nivel 2 gerou ')
                                    )
                               ),(write('Linha: '),write(LinWin),write(' Coluna: '),write(ColWin),nl)
                      )
                   ).
%actualiza o jogador
proximoJogador(Actual,Proximo):-       (Actual =:= 1,Proximo is 2); (Actual =:= 2, Proximo is 1).

%retorna a cor da casa onde se encontra a torre
corCasa(Jogador,Torre,Tab,PosTorres,CorCasa):-
                        torre(Jogador,Torre,NomeTorre),
                        membrotab(NomeTorre,Col,Lin,PosTorres),
                        membrotab(CorCasa,Col,Lin,Tab).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%             MOVIMENTO         %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
movimentar(Jogador,NomeTorre,Coluna,Linha,Tab,PosTorres,NovoTabuleiro):-
                      apaga_peca(NomeTorre,PosTorres,NewPosTorres),
                      move_peca(NomeTorre,Coluna,Linha,NewPosTorres,NovoTabuleiro),
                      ver_estado(Tab,NovoTabuleiro).

movimentar_aux(Jogador,NomeTorre,Coluna,Linha,Tab,PosTorres,NovoTabuleiro):-
                      apaga_peca(NomeTorre,PosTorres,NewPosTorres),
                      move_peca(NomeTorre,Coluna,Linha,NewPosTorres,NovoTabuleiro).



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%     VALIDACAO MOVIMENTO       %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
vertical(Col,Coldest,Lin,Lindest):- Lindest>0, Lindest<9,  Coldest=:=Col.
frente(Jogador,Col,Coldest,Lin,Lindest):-  Jogador =:= 1, Lindest>Lin ;
                                           Jogador =:= 2, Lindest<Lin.
diagonal(Col,Coldest,Lin,Lindest):- (Lindest-Lin)/(Coldest-Col) =:= 1 ; (Lindest-Lin)/(Coldest-Col) =:= -1 .

obstaculo_vertical(Jogador,Col,Coldest,Lin,Lindest,Tabuleiro):-   Jogador =:= 1, obstaculo_vertical2(Lin,Col,Lindest,Tabuleiro,1);
                                                                  Jogador =:= 2, obstaculo_vertical2(Lin,Col,Lindest,Tabuleiro,-1).

obstaculo_vertical2(Lindest,Col,Lindest,Tabuleiro,Delta).
obstaculo_vertical2(Count,Col,Lindest,Tabuleiro,Delta):-
       N is Count+Delta,
       livre(Col,N,Tabuleiro),
       obstaculo_vertical2(N,Col,Lindest,Tabuleiro,Delta).



obstaculo_diagonal(Jogador,Col,Coldest,Lin,Lindest,Tabuleiro):-   Jogador =:= 1,Coldest>Col,obstaculo_diagonal2(Lin,Col,Coldest,Lindest,Tabuleiro,1,1);
                                                                  Jogador =:= 1,Coldest<Col,obstaculo_diagonal2(Lin,Col,Coldest,Lindest,Tabuleiro,-1,1);
                                                                  Jogador =:= 2,Coldest>Col,obstaculo_diagonal2(Lin,Col,Coldest,Lindest,Tabuleiro,1,-1);
                                                                  Jogador =:= 2,Coldest<Col,obstaculo_diagonal2(Lin,Col,Coldest,Lindest,Tabuleiro,-1,-1).

obstaculo_diagonal2(Lindest,Coldest,Coldest,Lindest,Tabuleiro,DeltaCol,DeltaLin).
obstaculo_diagonal2(Count1,Count2,Coldest,Lindest,Tabuleiro,DeltaCol,DeltaLin):-
       N1 is Count1+DeltaLin,
       N2 is Count2+DeltaCol,
       livre(N2,N1,Tabuleiro),
       obstaculo_diagonal2(N1,N2,Coldest,Lindest,Tabuleiro,DeltaCol,DeltaLin).

livre(Coluna,Linha,Tabuleiro):- membrotab('  ',Coluna,Linha,Tabuleiro).

valida_movimento(Jogador,Col,Coldest,Lin,Lindest,Tabuleiro):-
                       frente(Jogador,Col,Coldest,Lin,Lindest), vertical(Col,Coldest,Lin,Lindest),obstaculo_vertical(Jogador,Col,Coldest,Lin,Lindest,Tabuleiro);
                       Col\=Coldest,frente(Jogador,Col,Coldest,Lin,Lindest), diagonal(Col,Coldest,Lin,Lindest),obstaculo_diagonal(Jogador,Col,Coldest,Lin,Lindest,Tabuleiro).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%       CASO TERMINAÇÂO       %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
avalia_tabuleiro(PosTorres,Valor):-
				(ganhou(1,PosTorres),Valor is 1); 
				(ganhou(2,PosTorres), Valor is 2 );
				Valor is 0.

%verifica se o Jogador ganhou o jogo
ganhou(Jogador,PosTorres):-
                    (Jogador =:= 1,Linha is 8  ; Jogador =:= 2,Linha is 1  ),
					(testa_linha(Jogador,PosTorres,Linha,0) -> fail; true).

testa_linha(Jogador,PosTorres,Linha,8).
testa_linha(Jogador,PosTorres,Linha,Coluna):-
                    NovaColuna is Coluna+1,
                    testa_torre(Jogador,Linha,Coluna,PosTorres,1),
                    testa_linha(Jogador,PosTorres,Linha,NovaColuna).

testa_torre(Jogador,Linha,Coluna,PosTorres,8).
testa_torre(Jogador,Linha,Coluna,PosTorres,X):-  torre(Jogador, X , NomeTorre),
												(membrotab(NomeTorre,Coluna,Linha,PosTorres) -> fail; true),                          
                                                 Z is X+1,
                                                 testa_torre(Jogador,Linha,Coluna,PosTorres,Z).
                                                 
                                                 

%verifica se a torre esta bloqueada
deadlock(Jogador,NomeTorre,PosTorres):-
                        ((Jogador =:= 1, Delta is 1); (Jogador =:= 2, Delta is -1)),
                         membrotab(NomeTorre,Colini,Linhini,PosTorres),
                        (
                              (NewCol is Colini+Delta,NewLin is Linhini+Delta,
                               valida_movimento(Jogador,Colini,NewCol,Linhini,NewLin,PosTorres) )
                            ;
                              (NewCol2 is Colini-Delta,NewLin2 is Linhini+Delta,
                               valida_movimento(Jogador,Colini,NewCol2,Linhini,NewLin2,PosTorres))
                            ;
                               (NewCol3 is Colini,NewLin3 is Linhini+Delta,
                               valida_movimento(Jogador,Colini,NewCol3,Linhini,NewLin3,PosTorres))
                         )
                        .
                        

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%          COMPUTADOR         %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
gerar_jogada(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,Nivel):-
                  ( Nivel =:=1 ,gerar_mov(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin) )
                   ;
                    (Nivel =:=2 , gerar_greedy(Jogador,NomeTorre,Tab,PosTorres,Coluna,Linha)).
%nivel 1 de dificuldade- quando pode ganhar ganha, senao faz random
gerar_mov(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin):-
                     gerar_mov_aux(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,0).


gerar_mov_aux(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,LinhaVitoria):-
                       possivel_ganhar(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,LinhaVitoria)
                       ;
                       (
                         random(0,7,NewLinhaVitoria),
                         gerar_mov_aux(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,NewLinhaVitoria)
                       ).

%gera uma jogada para tentar ganhar o jogo
possivel_ganhar(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,LinhaVitoria):-
                     membrotab(NomeTorre,Colini,Linhini,PosTorres),
                     (
                       (random(0,2,Random),Jogador =:= 1, Delta is 1,LinhaFinal is 8-LinhaVitoria, DeltaLinha is (LinhaFinal-Linhini))
                       ;
                       (random(0,2,Random),Jogador =:= 2, Delta is -1,LinhaFinal is 1+LinhaVitoria,DeltaLinha is (Linhini-LinhaFinal))
                     ),
                    (
                       ( Random=:=0,(
                           testa_diagonal1(Jogador,Colini,TrialCol,Linhini,LinhaFinal,PosTorres,DeltaLinha,ColWin,LinWin);
                           testa_diagonal2(Jogador,Colini,TrialCol,Linhini,LinhaFinal,PosTorres,DeltaLinha,ColWin,LinWin);
                           testa_vertical(Jogador,Colini,Linhini,LinhaFinal,PosTorres,ColWin,LinWin) )
                        )
                        ;
                        (  Random=:=1,(
                             testa_diagonal2(Jogador,Colini,TrialCol,Linhini,LinhaFinal,PosTorres,DeltaLinha,ColWin,LinWin);
                             testa_diagonal1(Jogador,Colini,TrialCol,Linhini,LinhaFinal,PosTorres,DeltaLinha,ColWin,LinWin);
                             testa_vertical(Jogador,Colini,Linhini,LinhaFinal,PosTorres,ColWin,LinWin) )
                       )
                        ;
                        (   Random=:=2,(
                                testa_vertical(Jogador,Colini,Linhini,LinhaFinal,PosTorres,ColWin,LinWin);
                                testa_diagonal2(Jogador,Colini,TrialCol,Linhini,LinhaFinal,PosTorres,DeltaLinha,ColWin,LinWin);
                                testa_diagonal1(Jogador,Colini,TrialCol,Linhini,LinhaFinal,PosTorres,DeltaLinha,ColWin,LinWin)  )
                       )
                    ).

%testa se e possivel deslocar a torre na diagonal 1
 testa_diagonal1(Jogador,Colini,TrialCol,Linhini,LinhaFinal,PosTorres,DeltaLinha,ColWin,LinWin):-
                        TrialCol is (Colini+DeltaLinha) ,
                        valida_movimento(Jogador,Colini,TrialCol,Linhini,LinhaFinal,PosTorres),
                        ColWin is TrialCol,
                        LinWin is LinhaFinal.

%testa se e possivel deslocar a torre na diagonal 2
 testa_diagonal2(Jogador,Colini,TrialCol,Linhini,LinhaFinal,PosTorres,DeltaLinha,ColWin,LinWin):-
                        TrialCol is (Colini-DeltaLinha),
                        valida_movimento(Jogador,Colini,TrialCol,Linhini,LinhaFinal,PosTorres),
                        ColWin is TrialCol,
                        LinWin is LinhaFinal.
                        
%testa se e possivel deslocar a torre na vertical
 testa_vertical(Jogador,Colini,Linhini,LinhaFinal,PosTorres,ColWin,LinWin):-
                        valida_movimento(Jogador,Colini,Colini,Linhini,LinhaFinal,PosTorres),
                        ColWin is Colini,
                        LinWin is LinhaFinal.

%nivel 2 de dificuldade
%se poder ganhar joga para ganhar, se nao
%escolhe umas das jogadas possiveis que impedem que o adversario ganhe na jogada seguinte
gerar_greedy(Jogador,NomeTorre,Tab,PosTorres,Coluna,Linha):-
                     (
                       gerar_possibilidades(Jogador,NomeTorre,Tab,PosTorres,ListaColunas,ListaLinhas),
                       gerar_greedy_aux(Jogador,NomeTorre,Tab,PosTorres,ListaColunas,ListaLinhas,[],[],NewListaColunas,NewListaLinhas)
                      ),
                     (
                      possivel_ganhar(Jogador,NomeTorre,Tab,PosTorres,Coluna,Linha,0)
                      ;
                      peca_aleatoria(NewListaColunas,NewListaLinhas,Coluna,Linha)
                     ).

gerar_greedy_aux(Jogador,NomeTorre,Tab,PosTorres,[],[],ColunaFinal,LinhaFinal,ColunaFinal,LinhaFinal).
gerar_greedy_aux(Jogador,NomeTorre,Tab,PosTorres,[HColuna|RestoColuna],[HLinha|RestoLinha],ColunaFinal,LinhaFinal,NewColunaFinal,NewLinhaFinal):-
                            (
                              membrotab(NomeTorre,Colini,Linhini,PosTorres),
                              valida_movimento(Jogador,Colini,HColuna,Linhini,HLinha,PosTorres),
                              movimentar_aux(Jogador,NomeTorre,HColuna,HLinha,Tab,PosTorres,NovoTabuleiro),
                              proximoJogador(Jogador,ProximoJogador),
                              membrotab(NomeTorre,Col,Lin,NovoTabuleiro),
                              membrotab(CorCasa,Col,Lin,Tab) ,
                              torre(ProximoJogador,CorCasa,TorreProximoJogador)
                           ),
                           (
                           (
                              pode_ganhar(ProximoJogador,TorreProximoJogador,Tab,NovoTabuleiro),
                              gerar_greedy_aux(Jogador,NomeTorre,Tab,PosTorres,RestoColuna,RestoLinha,ColunaFinal,LinhaFinal,NewColunaFinal,NewLinhaFinal)
                            )
                             ;
                             (
                                append([HColuna],ColunaFinal,NovaColunaFinal),
                                append([HLinha],LinhaFinal,NovaLinhaFinal),
                                gerar_greedy_aux(Jogador,NomeTorre,Tab,PosTorres,RestoColuna,RestoLinha,NovaColunaFinal,NovaLinhaFinal,NewColunaFinal,NewLinhaFinal)
                              )
                           ).


%gera todas as possibilidades de jogadas
gerar_possibilidades(Jogador,NomeTorre,Tab,PosTorres,ListaColunas,ListaLinhas):-
                    gerar_possibilidades_aux(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,0,0,[],[],ListaColunas,ListaLinhas).


gerar_possibilidades_aux(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,7,Sequence,ListaColunas,ListaLinhas,ListaColunas,ListaLinhas).
gerar_possibilidades_aux(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,LinhaVitoria,Sequence,ListaColunas,ListaLinhas,ListaColunasFinal,ListaLinhasFinal):-
                         (
                            (Sequence =:= 3, NewSequence is 0, NewLinhaVitoria is LinhaVitoria+1)
                             ;
                             (NewSequence is Sequence+1,NewLinhaVitoria is LinhaVitoria)
                         ),
                         (
                           (
                            jogada_possivel(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,NewLinhaVitoria,NewSequence),
                            append([ColWin],ListaColunas,NewListaColunas),
                            append([LinWin],ListaLinhas,NewListaLinhas),
                            gerar_possibilidades_aux(Jogador,NomeTorre,Tab,PosTorres,NewColWin,NewLinWin,NewLinhaVitoria,NewSequence,NewListaColunas,
                            NewListaLinhas,ListaColunasFinal,ListaLinhasFinal)
                            )
                           ;
                          (
                          gerar_possibilidades_aux(Jogador,NomeTorre,Tab,PosTorres,NewColWin,NewLinWin,NewLinhaVitoria,NewSequence,ListaColunas,
                            ListaLinhas,ListaColunasFinal,ListaLinhasFinal)
                          )
                        ).

%verifica se e possivel ganhar nessa jogada
pode_ganhar(Jogador,NomeTorre,Tab,PosTorres):-
                  jogada_possivel(Jogador,NomeTorre,Tab,PosTorres,ColWin1,LinWin1,0,0);
                  jogada_possivel(Jogador,NomeTorre,Tab,PosTorres,ColWin2,LinWin2,0,1);
                  jogada_possivel(Jogador,NomeTorre,Tab,PosTorres,ColWin3,LinWin3,0,2).
                  

%devolve uma jogada possivel
jogada_possivel(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin,LinhaVitoria,Sequence):-
                     membrotab(NomeTorre,Colini,Linhini,PosTorres),
                     (
                       (Jogador =:= 1, Delta is 1,LinhaFinal is 8-LinhaVitoria, DeltaLinha is (LinhaFinal-Linhini))
                       ;
                       (Jogador =:= 2, Delta is -1,LinhaFinal is 1+LinhaVitoria,DeltaLinha is (Linhini-LinhaFinal))
                     ),
                    (
                          ( Sequence=:=0 ,
                          testa_diagonal1(Jogador,Colini,TrialCol,Linhini,LinhaFinal,PosTorres,DeltaLinha,ColWin,LinWin))
                          ;
                          ( Sequence=:=1 ,
                            testa_diagonal2(Jogador,Colini,TrialCol,Linhini,LinhaFinal,PosTorres,DeltaLinha,ColWin,LinWin))
                          ;
                          ( Sequence=:=2 ,
                            testa_vertical(Jogador,Colini,Linhini,LinhaFinal,PosTorres,ColWin,LinWin) )
                    ).
%escolhe aleatoriamente um dos elementos da lista de pecas
%nota: Lista1 sao as colunas, e Lista2 as linhas respectivas
peca_aleatoria(Lista1,Lista2,Elemento1,Elemento2):-
                  length(Lista1,Nelementos),
                  random(1,Nelementos,Indice),
                  nth1(Indice,Lista1,Elemento1), nth1(Indice,Lista2,Elemento2).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%      MOVIMENTO PEÇA       %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        
membrotab(Pec,X,Y,Tab):-
        membro_pos_lista(Linha, Y, Tab),
        membro_pos_lista(Pec, X, Linha).
       

membro_pos_lista(Membro, N, Lista):-
        membro_pos_procura(Membro, 1, N, Lista).

membro_pos_procura(Membro, N, N, [Membro|_]).
membro_pos_procura(Membro, P, N, [_|T]):-
        P2 is P+1,
        membro_pos_procura(Membro, P2, N, T).



%move_peca(+Torre,+Coluna,+Linha,+Tab,-NovoTab)
move_peca(Torre,X,Y,Tab,NovoTab):-
        move_peca2(1,'  ',Torre,X,Y,Tab,NovoTab),!.

move_peca2(_,_,_,_,_,[],[]).
move_peca2(Y,Peca,Pnov,X,Y,[Lin|Resto],[NovLin|Resto2]):-
        procura_linha(1,Peca,Pnov,X,Lin,NovLin),
        N2 is Y+1,
        move_peca2(N2,Peca,Pnov,X,Y,Resto,Resto2).
move_peca2(N,Peca,Pnov,X,Y,[Lin|Resto],[Lin|Resto2]):-
        N\=Y, N2 is N+1,
        move_peca2(N2,Peca,Pnov,X,Y,Resto,Resto2).

procura_linha(_,_,_,_,[],[]).
procura_linha(X,Peca,Pnov,X,[Peca|Resto],[Pnov|Resto2]):-
        N2 is X+1,
        procura_linha(N2,Peca,Pnov,X,Resto,Resto2).
procura_linha(N,Peca,Pnov,X,[El|Resto],[El|Resto2]):-
        N\=X, N2 is N+1,
        procura_linha(N2,Peca,Pnov,X,Resto,Resto2).
        
        
%apaga_peca(+Peca,+Tab,-NovoTab)
apaga_peca(Peca,Tab,NovoTab):-
        apaga_peca2(Peca,'  ',Tab,NovoTab),!.

apaga_peca2(_,_,[],[]).
apaga_peca2(Peca,Pnov,[Lin|Resto],[NovLin|Resto2]):-
        apaga_pecalinha(Peca,Pnov,Lin,NovLin),
        apaga_peca2(Peca,Pnov,Resto,Resto2).
apaga_peca2(Peca,Pnov,[Lin|Resto],[Lin|Resto2]):-
        apaga_peca2(Peca,Pnov,Resto,Resto2).

apaga_pecalinha(_,_,[],[]).
apaga_pecalinha(Peca,Pnov,[Peca|Resto],[Pnov|Resto2]):-
        apaga_pecalinha(Peca,Pnov,Resto,Resto2).
apaga_pecalinha(Peca,Pnov,[El|Resto],[El|Resto2]):-
        apaga_pecalinha(Peca,Pnov,Resto,Resto2).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%  VISUALIZACAO DO ESTADO DO JOGO                 %%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%visualiza_estado(Tabuleiro)
ver_estado(Tab,PosTorres):-
        write('     1    2    3    4    5    6    7    8'),nl,
        write('   ---------------------------------------'),nl ,
        mostra_linhas(Tab,PosTorres,1),nl,nl.

mostra_linhas([],[],Count).
mostra_linhas([Lin|Resto],[Lin2|Resto2],Count):-
        write('  |'),mostra_linha(Lin2),nl,
        write(Count),write(' |'),mostra_linha2(Lin),nl,
        write('   ---------------------------------------'),nl,
        NewCount is Count+1,
        mostra_linhas(Resto,Resto2,NewCount).

mostra_linha([]).
mostra_linha([EL1|Resto]):-
        write(EL1),write('  |'),
        mostra_linha(Resto).

mostra_linha2([]).
mostra_linha2([EL1|Resto]):-
        write('   '),write(EL1),write('|'),
        mostra_linha2(Resto).
        
        
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%           VALIDAÇÃO INPUT        %%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
input(X) :- get_code(X).
convN(Let,Valor):- numero(Let), Valor is Let-48.
numero(Let):- Let>=48, Let=<57.
espaco(Let):- Let =:=32.

mes(Inf, Sup):- write('Caracter invalido - valor entre '), write(Inf), write(' e '), write(Sup) , nl.

valida_jogada_numero(Inf, Sup, Entrada):-  input(SX), convN(SX, Entrada), (Entrada >= Inf) , (Entrada =< Sup).

insiste_fail(Inf, Sup, Entrada) :- (mes(Inf, Sup), valida_jogada_numero(Inf, Sup, New_entrada)) ; insiste(Inf, Sup, NNewEntrada).

insiste(Inf, Sup, Entrada):-
valida_jogada_numero(Inf, Sup, Entrada)
                                        ;
                                                        insiste_fail(Inf, Sup, New_entrada).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  SOCKET %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

port(60070).


server:-
        port(Port),
        socket_server_open(Port,Socket),
        socket_server_accept(Socket, _Client, Stream, [type(text)]),
        server_loop(Stream),
        socket_server_close(Socket),
        write('Server Exit'),nl.

server_loop(Stream) :-
        repeat,
                read(Stream, ClientRequest),
                write('Received: '), write(ClientRequest), nl, 
                server_input(ClientRequest, ServerReply),
                format(Stream, '~q.~n', [ServerReply]),
                write('Send: '), write(ServerReply), nl, 
                flush_output(Stream),
        (ClientRequest == bye; ClientRequest == end_of_file), !.

server_input(inicio, ok(['inicio do jogo'])):- 
  inicio,!.

server_input(inicializaTab(Num), ok(Tab)):-
                                                        tabuleiro(Tab),!.

server_input(inicializaPosTorres(Num), ok(PosTorres)):-
                                                        postorres(PosTorres),!.
														
server_input(jogada_humano(PosTorres), ok(Tab)):-
						tabuleiro(Tab),!.
					                               
  
server_input(valida(PosTorres,NumeroTorre,Linha,Coluna,Jogador), ok(Res)):-
						tabuleiro(Tab),
						write('Jogador: '),write(Jogador),nl,
						write('Torre: '),write(NumeroTorre),nl,
						torre(Jogador,NumeroTorre,NomeTorre),nl,
						write('Torre: '),write(NomeTorre),
						membrotab(NomeTorre,Colini,Linhini,PosTorres),
                        (valida_movimento(Jogador,Colini,Coluna,Linhini,Linha,PosTorres) -> Res is 1; Res is 0),!.
						
server_input(execute(PosTorres,NumeroTorre,Linha,Coluna,Jogador), ok(NovoTabuleiro,ProxJogador,ProxTorre)):- 
    tabuleiro(Tab),
    torre(Jogador,NumeroTorre,NomeTorre),
    movimentar_aux(Jogador,NomeTorre,Coluna,Linha,Tab,PosTorres,NovoTabuleiro),
    proximoJogador(Jogador,ProxJogador),
   corCasa(Jogador,NumeroTorre,Tab,NovoTabuleiro,ProxTorre),!.
        
server_input(calculate(PosTorres,Nivel,Jogador,NumeroTorre), ok(NovoTabuleiro,ProxJogador,ProxTorre,ColWin,LinWin)):- 
    tabuleiro(Tab),
    torre(Jogador,NumeroTorre,NomeTorre),
     ((         Nivel=:=1,
               gerar_mov(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin)
        )
          ;
              gerar_greedy(Jogador,NomeTorre,Tab,PosTorres,ColWin,LinWin)
     ),
    movimentar(Jogador,NomeTorre,ColWin,LinWin,Tab,PosTorres,NovoTabuleiro),
    proximoJogador(Jogador,ProxJogador),
    corCasa(Jogador,NumeroTorre,Tab,NovoTabuleiro,ProxTorre),!.
        
server_input(game_end(PosTorres), ok(JogadorVitorioso)):- 
    tabuleiro(Tab),
	avalia_tabuleiro(PosTorres,JogadorVitorioso),!.
	
server_input(blocked(PosTorres,Jogador,CorTorre), ok(Presa,ProximoJogador,NTorre)):- 	
					torre(Jogador,CorTorre,NomeTorre),	
					(
					(	deadlock(Jogador,NomeTorre,PosTorres),
						ProximoJogador is 1,
						NTorre is 1,
						Presa is 0
					)
					;
						tabuleiro(Tab),
						proximoJogador(Jogador,ProximoJogador),
						corCasa(Jogador,CorTorre,Tab,PosTorres,NTorre),
						Presa is 1
					),!.
        
server_input(bye, ok):-!.
server_input(end_of_file, ok):-!.
%erver_input(_, invalid) :- !.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%  REPRESENTACAO DO ESTADO INICIAL %%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%carregar tabuleiro
estado(Tab,PosTorres):- postorres(PosTorres),tabuleiro(Tab).

%posicao inicial das torres
postorres([ ['X1','X2','X3','X4','X5','X6','X7','X8'],
            ['  ','  ','  ','  ','  ','  ','  ','  '],
            ['  ','  ','  ','  ','  ','  ','  ','  '],
            ['  ','  ','  ','  ','  ','  ','  ','  '],
            ['  ','  ','  ','  ','  ','  ','  ','  '],
            ['  ','  ','  ','  ','  ','  ','  ','  '],
            ['  ','  ','  ','  ','  ','  ','  ','  '],
            ['Y8','Y7','Y6','Y5','Y4','Y3','Y2','Y1']]).
            
%cores das casas do tabuleiro
tabuleiro([[1,2,3,4,5,6,7,8],
           [6,1,4,7,2,5,8,3],
           [7,4,1,6,3,8,5,2],
           [4,3,2,1,8,7,6,5],
           [5,6,7,8,1,2,3,4],
           [2,5,8,3,6,1,4,7],
           [3,8,5,2,7,4,1,6],
           [8,7,6,5,4,3,2,1]]).

%torres do jogador 1
%torre(+Jogador,+NumeroTorre,-NomeTorre)
torre(1,1,'X1').
torre(1,2,'X2').
torre(1,3,'X3').
torre(1,4,'X4').
torre(1,5,'X5').
torre(1,6,'X6').
torre(1,7,'X7').
torre(1,8,'X8').

%torres do jogador 2
torre(2,1,'Y1').
torre(2,2,'Y2').
torre(2,3,'Y3').
torre(2,4,'Y4').
torre(2,5,'Y5').
torre(2,6,'Y6').
torre(2,7,'Y7').
torre(2,8,'Y8').