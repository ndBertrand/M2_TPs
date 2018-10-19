
##Compte rendu TP1 Administration des bases de données - TP1

#Analyse d'une instance Oracle existance

1. connection à une instance oracle
>  Xhost + 
ssh -x oracle@172.16.36.132
password : oracle
sqlplus  / as sysdba

2. nom de l'instance surlequel on est connecté
> SELECT instance_name from v$instance
orcl

3. nom de la base de données, le nom de l'instance et la taille des blocs de la base de données
> SELECT database_name from v$database
ORCL

>select value from v$parameter where name = 'db_block_size';
8192

4. les noms et les tailles des fichiers de données
> select name,blocks from v$datafile

/u01/app/oracle/oradata/orcl/system01.dbf
     87040

/u01/app/oracle/oradata/orcl/sysaux01.dbf
     67840

/u01/app/oracle/oradata/orcl/undotbs01.dbf
     13440


/u01/app/oracle/oradata/orcl/users01.dbf
       640

/u01/app/oracle/oradata/orcl/example01.dbf
     12800


6. Affache du numero de version 
> select version from v$instance

7. le nombre de paramètrer
slect value from v$parameter where name  ="processes'
>150

8.
9. 
DB_BLOCK_SIZE must be 8192 to mount this database (not 16384)

10. énumaration des paramètres d'initialisation par défaut.
NUM						    NUMBER
 NAME						    VARCHAR2(80)
 TYPE						    NUMBER
 VALUE						    VARCHAR2(4000)
 DISPLAY_VALUE					    VARCHAR2(4000)
 ISDEFAULT					    VARCHAR2(9)
 ISSES_MODIFIABLE				    VARCHAR2(5)
 ISSYS_MODIFIABLE				    VARCHAR2(9)
 ISINSTANCE_MODIFIABLE				    VARCHAR2(5)
 ISMODIFIED					    VARCHAR2(10)
 ISADJUSTED					    VARCHAR2(5)
 ISDEPRECATED					    VARCHAR2(5)
 ISBASIC					    VARCHAR2(5)
 DESCRIPTION					    VARCHAR2(255)
 UPDATE_COMMENT 				    VARCHAR2(255)
 HASH						    NUMBER


11. connection en tant que scott/tiger

connect scott/tiger
ou
sqlplus scott/tiger

INSERT INTO EMP (EMPNO,ENAME,JOB,MGR,SAL,DEPTNO)VALUES(800,'DUPONT','ANALYST',7566,1500,20);

INSERT INTO EMP (EMPNO,ENAME,JOB,MGR,SAL,DEPTNO)VALUES(800,'DUJARDIN','ANALYST',7566,1500,20);



