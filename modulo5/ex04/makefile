# exemplo de makefile com variaveis
# variaveis
INCLUDES = 
NAME = ex04
SOURCES = ${NAME}.c 
OBJFILES = ${NAME}.o
EXEC = ${NAME}
CFLAGS=-lrt
CFLAGSTHREAD=-lpthread


# Mensagem recebida por parametros
MSG = ${msg}

# Modulo recebido por parametros
MOD=MODULO${mod}

# regras de sufixo
.SUFFIXES : .c .o
.SUFFIXES : .s .o

${OBJFILES}: ${SOURCES} ${INCLUDES}

# como transformar um .c num .o ; $< -- nome do ficheiro
.c.o:
	gcc -Wall -g -c $< $(CFLAGSTHREAD) $(CFLAGS)
	
# como transformar um .c num .o ; $< -- nome do ficheiro
.s.o:
	gcc -Wall -g -c $<
	
all: ${EXEC}

${EXEC}: ${OBJFILES}
	gcc -Wall -g -o ${EXEC} ${OBJFILES} $(CFLAGSTHREAD) $(CFLAGS)

run:${EXEC}
	./${EXEC}

top:clear
	kill
	
clean:
	rm -f ${OBJFILES} ${EXEC}	

#apagar semaforos ...para quando dá erro
clear:
	rm /dev/shm/s*
	
# Configuração do GIT
gitconfig:
	git config --global user.email "hplive@hotmail.com"
	git config --global user.name "helder pereira"
	git config --global push.default simple
	
# Update do GIT
git:
	git add -A
	git commit -a -m "${MSG}"
	git pull
	git push

githp:
	make clean
	git add -A
	git commit -a -m "${MOD} ${NAME}"
	git pull
	git push

#kill processes when stuck
kill:
	pkill -f ${NAME}


#debug:
#	ddd ${EXE} &