# Nome do executável
EXEC = mergesort

# Compilador
CC = gcc

# Opções de compilação (bibliotecas)
LIBS = -pthread -Wall -Wextra

# Arquivos fonte e resultados objeto
FONTES = $(wildcard *.c)
OBJETOS = $(FONTES:.c=.o)

# Regra principal
all: $(EXEC)

# Regra para criar o executável
$(EXEC): $(OBJETOS)
	$(CC) $(LIBS) -o $@ $^

# Regra para compilar os arquivos objeto
%.o: %.c funcoes.h
	$(CC) $(LIBS) -c $< -o $@

# Regra para executar o programa
run: $(EXEC)
	./$(EXEC)

# Limpar os arquivos gerados pela compilação
clean:
	rm -f $(OBJETOS) $(EXEC)