
##Compte rendu TP1 Administration des bases de données - TP1

#Analyse d'une instance Oracle existance

1. connection à une instance oracle
>  Xhost + 
ssh -x oracle@172.16.36.132
password : oracle
sqlplus  / as sysdba

2. nom de l'instance surlequel on est connecté
> SELECT instance_name from v$instance;
orcl

3. nom de la base de données, le nom de l'instance et la taille des blocs de la base de données
> SELECT database_name from v$database;
ORCL

>select value from v$parameter where name = 'db_block_size';
8192

4. les noms et les tailles des fichiers de données
> select name,blocks from v$datafile;

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
select value from v$parameter where name  ='processes';
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


# CREATION D'UNE BASE DE DONNEES

1. question 1,2,3 faire des mkdir dans les dossier correspondant

4. 
- copier coller le fichier initorcl165645.ora se trouvant dans /u01/app/oracle/admin/orcl/pfile
   dans le dossier /u01/app/oracle/admin/myinst/pfile puis le renomer initmyinst.ora
- remplacer toutes les occurences "orcl" se trouvant dans initmyinst.ora par "myinst"
- commenter la ligne  :
  undo_tablespace=UNDOT......
- ajouter la ligne 
  instance_name = myinst (en bas de db_name = myinst)

5. Modifier le script crationBD.sql en comme indiqué dans le tp mais en laissant les tailles

'''
CREATE DATABASE myinst USER SYS IDENTIFIED BY oracle USER SYSTEM IDENTIFIED BY oracle LOGFILE
GROUP 1 '/u01/app/oracle/oradata/myinst/myinst_log1a.log' SIZE 100M,
GROUP 2 '/u01/app/oracle/oradata/myinst/myinst_log2a.log' SIZE 100M
MAXLOGFILES 5
MAXLOGMEMBERS 5
MAXLOGHISTORY 1
MAXDATAFILES 30
MAXINSTANCES 1
CHARACTER SET US7ASCII
NATIONAL CHARACTER SET AL16UTF16
DATAFILE '/u01/app/oracle/oradata/myinst/system01myinst.dbf' SIZE 100M
REUSE
EXTENT MANAGEMENT LOCAL
SYSAUX DATAFILE '/u01/app/oracle/oradata/myinst/sysaux01.dbf' SIZE
325M REUSE
DEFAULT TEMPORARY TABLESPACE tempts1
TEMPFILE '/u01/app/oracle/oradata/myinst/temp01.dbf'
SIZE 20M REUSE
UNDO TABLESPACE undotbs
DATAFILE '/u01/app/oracle/oradata/myinst/undotbs01.dbf' SIZE 200M REUSE AUTOEXTEND ON MAXSIZE UNLIMITED;
'''

6. Démarrage de l'instance en mode nomount :
>  startup nomount pfile=/u01/app/oracle/admin/myinst/pfile/initmyinst.ora

puis éxecuter le script de création de la base de données (ci-dessus)


7. monter du fichier
>  startup  upgrade pfile=/u01/app/oracle/admin/myinst/pfile/initmyinst.ora

verfication des fichiers de base de données : 
select name,blocks from v$datafile;

> /u01/app/oracle/oradata/myinst/system01myinst.dbf
     12800

/u01/app/oracle/oradata/myinst/sysaux01.dbf
     41600

/u01/app/oracle/oradata/myinst/undotbs01.dbf
     25600

8. affichage des utilisateurs de la base de données :

> select * from all_users

# CREATION DE VUES DU DICTIONNAIRE DE DONN2ES ET CREATION DES PACKAGES STANDARD

1. start /u01/app/oracle/product/11.2.0.4/db_1/rdbms/admin/catalog.sql

2. select username from dba_users;

3. start /u01/app/oracle/product/11.2.0.4/db_1/rdbms/admin/catproc.sql

3. start /u01/app/oracle/product/11.2.0.4/db_1/rdbms/admin/utlsampl.sql




select owner, table_name, tablespace_name from dba_tables where owner='SCOTT';

