#seção crítica: não pode ter solicitações em paralelo ou entrelaçadas, duas ou mais threads não podem executar uma seção crítica ao mesmo tempo.

#Exclusão mútua: garante que apenas uma thread por vez entre na seção crítica.

#Deadlock: é a trava, o passe que libera uma thread entrar na seção crítica, apenas uma thread/processo obterá sucesso.

#Spinlock: não existe entrada eventual garantida. Pois, não existe escalonador justo. O spinlock é uma variável ou trava que determina se uma thread/processo deve entrar na seção crítica ou não.
    
#Test and Set: teste de trava para a seção a crítica. Altera o valor de uma variável para modificar o contexto da seção crítica e a entrada de threads/processos na seção.
