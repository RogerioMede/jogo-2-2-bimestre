# Dungeon Crawler

## O que é o jogo
Dungeon Crawler é um jogo simples de aventura em C. O objetivo é:
1. Começar na Vila, aprender a se mover e conversar com NPCs.
2. Coletar chaves em cada fase para abrir portas.
3. Evitar espinhos e monstros.
4. Avançar por três fases de masmorra até vencer.

## História
Você é um herói em uma pequena vila. Dois NPCs ensinam como jogar:
- **NPC 1**: mostra como usar W/A/S/D para se mover.
- **NPC 2**: explica que é preciso pegar a chave “@” para abrir a porta “D”.

Depois de coletar a chave na vila, você entra na Fase 1. Cada fase seguinte tem desafios novos (espinhos, monstros, botão, teleporte), mas sempre a mesma ideia: pegar a chave e andar sobre a porta aberta para avançar.

## Regras e Objetivo
- **Objetivo principal**: chegar à última fase e vencer.
- Cada fase tem uma chave (“@”) e uma porta (“D”):
  - Ao andar sobre a chave, ela some e a porta muda para “=”.
  - Basta andar sobre “=” para ir para a próxima fase (ou vencer, na Fase 3).
- Se encostar em espinhos (“#”) ou monstros (“X” ou “V”), a fase reinicia.
- Você tem até 3 tentativas por fase. Após 3 mortes, volta ao menu principal.

## Como Jogar
- **Controles**:
  - W = cima  
  - A = esquerda  
  - S = baixo  
  - D = direita  
  - I = interagir (falar com NPCs na Vila ou apertar botão nas fases)

- **Fases**:
  1. **Vila (10×10)**  
     - NPCs “P”. Fique ao lado e pressione `I` para dicas.  
     - Chave “@”. Ande sobre ela para coletar e abrir a porta “D”.  
     - Ande sobre “=” (porta aberta) para entrar na Fase 1.  
  2. **Fase 1 (10×10)**  
     - Chave “@”. Porta “D” vira “=” quando você pega a chave.  
     - Ande sobre “=” para ir à Fase 2.  
  3. **Fase 2 (20×20)**  
     - Chave “@ ,Porta “D”.  
     - Botão “O” surge 2 espinhos aleatorios.  
     - Espinhos iniciais. Monstros “X”.    
  4. **Fase 3 (40×40)**  
     - Chave “@”. Porta “D”.    
     - Espinhos iniciais em dez posições espalhadas. Monstros “X”: três, movem-se aleatoriamente. Monstro “V”: persegue em linha reta.  
     - Teleporte “>”.  
     - Ande sobre “=” para vencer.
