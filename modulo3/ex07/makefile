# exemplo de makefile com variaveis
# variaveis
INCLUDES = 
NAME = ex07
NAME2 = random
NAME3 = 
SOURCES = ${NAME}.c ${NAME2}.c
OBJFILES = ${NAME}.o ${NAME2}.o
EXEC = ${NAME}
CFLAGS =-lrt

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
	gcc -Wall -g -c $< $(CFLAGS)
	
# como transformar um .c num .o ; $< -- nome do ficheiro
.s.o:
	gcc -Wall -g -c $<
	
all: ${EXEC}

${EXEC}: ${OBJFILES}
	gcc -Wall -g -o ${EXEC} ${OBJFILES} $(CFLAGS)

run: ${EXEC}
	./${EXEC}

clean:
	rm -f ${OBJFILES} ${EXEC}
	
# Configuração do GIT
gitconfig:
	git config --global user.email "1151052@isep.ipp.pt"
	git config --global user.name "BrunoPatrao"
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


#debug:
#	ddd ${EXE} &