#MAKEFILE

INCLUDES=
SOURCES=prdemo02.c
OBJFILES=prdemo02.o
EXEC=prog
MSG=$(msg)
MOD=MODULO$(mod)
EX=EXERCICIO$(ex)

#regras de sufixo
.SUFFIXES : .c .o
.SUFFIXES : .s .o


#transformar um .c em .o
.c.o:
	gcc -Wall -g -c $<
	
.s.o:
	gcc -Wall -g -c $<
	
${EXEC}: ${OBJFILES}
	gcc -Wall -g -o ${EXEC} ${OBJFILES}
	
${OBJFILES}:${SOURCES} ${INCLUDES}

run: ${EXEC}
	./${EXEC}
	
clean:
	rm -f ${OBJFILES} ${EXEC}
	
#configuraçao do git
gitconfig:
	git config --global user.email "hplive@hotmail.com"
	git config --global user.name "helder pereira"
	git config --global push.default simple

#Actualização do Git
#make git msg="mensagem"
git:
	make clean
	git add -A
	git commit -a -m "${MSG}"
	git pull
	git push
	
githp:
	make clean
	git add -A
	git commit -a -m "${MOD} ${EX}"
	git pull
	git push

#debug
debug: ${EXEC}
	ddd ${EXEC} &
	
#teclado portugues
teclado:
	setxkbmaps pt


	
