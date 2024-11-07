# Nome do executavel
EXEC = mergesort

# Compilador
CC = gcc

# Opções de compilacao (bibliotecas)
LIBS = -pthread

# Arquivos fonte e resultados objeto
FONTES = $(wildcard *.c)
OBJETOS = $(FONTES:.c=.o)

# Regra principal
all: $(EXEC)

# Regra para criar o executavel
$(EXEC): $(OBJETOS)
	$(CC) $(LIBS) -o $@ $^

# Regra para compilar os arquivos objeto
%.o: %.c funcoes.h
	$(CC) $(LIBS) -c $< -o $@

# Regra para executar o programa
run: $(EXEC)
	./$(EXEC)

# Limpar os arquivos gerados pela compilacao
clean:
	rm -f $(OBJETOS) $(EXEC)