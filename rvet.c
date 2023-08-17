/**
 * Código base (incompleto) para implementação de relógios vetoriais.
 * Meta: implementar a interação entre três processos ilustrada na figura
 * da URL: 
 * 
 * https://people.cs.rutgers.edu/~pxk/417/notes/images/clocks-vector.png
 * 
 * Compilação: mpicc -o rvet rvet.c
 * Execução:   mpiexec -n 3 ./rvet
 */
 
#include <stdio.h>
#include <string.h>  
#include <mpi.h>     


typedef struct Clock { 
   int p[3];
} Clock;


void Event(int pid, Clock *clock){
   clock->p[pid]++;   
}


void Send(int pid, int destino, Clock *clock){
   clock->p[pid]++;
   int mensagem[3]; 
   mensagem[0] = clock->p[0];
   mensagem[1] = clock->p[1];
   mensagem[2] = clock->p[2];
   
   //MPI SEND
   MPI_Send(&mensagem, 3, MPI_INT, destino, 0, MPI_COMM_WORLD);
}

void Receive(int pid, int origem, Clock *clock){
   clock->p[pid]++;
   int mensagem[3];
   
   //MPI RECV
   MPI_Recv(&mensagem, 3, MPI_INT, origem, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   
   if (clock->p[0] < mensagem[0]) { clock->p[0] = mensagem[0]; }
   if (clock->p[1] < mensagem[1]) { clock->p[1] = mensagem[1]; }
   if (clock->p[2] < mensagem[2]) { clock->p[2] = mensagem[2]; }
}

// Representa o processo de rank 0
void process0(){
   Clock clock = {{0,0,0}};
   //a
   Event(0, &clock);
   printf("a)Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   //b
   Send(0, 1, &clock);
   printf("b)Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   //c
   Receive(0, 1, &clock);
   printf("c)Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   //d
   Send(0, 2, &clock);
   printf("d)Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   //e
   Receive(0, 2, &clock);
   printf("e)Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   //f
   Send(0, 1, &clock);
   printf("f)Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   //g
   Event(0, &clock);
   printf("g)Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
}

// Representa o processo de rank 1
void process1(){
   Clock clock = {{0,0,0}};
   //h
   Send(1, 0, &clock);
   printf("h)Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   //i
   Receive(1, 0, &clock);
   printf("i)Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   //j
   Receive(1, 0, &clock);
   printf("j)Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
}

// Representa o processo de rank 2
void process2(){
   Clock clock = {{0,0,0}};
   //k
   Event(2, &clock);
   printf("k)Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   //l
   Send(2, 0, &clock);
   printf("l)Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   //m
   Receive(2, 0, &clock);
   printf("m)Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
}

int main(void) {
   int my_rank;               

   MPI_Init(NULL, NULL); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank == 0) { 
      process0();
   } else if (my_rank == 1) {  
      process1();
   } else if (my_rank == 2) {  
      process2();
   }

   /* Finaliza MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
