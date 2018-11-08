Bertrand NDAYISENGA
Master 2 Informatique
Systèmes d'information et d'aide à la décision
Année Académique : 2017-2018

# Administration des bases de données - TP1
## Compte rendu

### Analyse d'une instance Oracle existance

1. connection à une instance oracle
```bash
Xhost + 
ssh -x oracle@172.16.36.132
password : oracle
sqlplus  / as sysdba
```
2. nom de l'instance surlequel on est connecté
```SQL
SELECT instance_name from v$instance;
INSTANCE_NAME
----------------
orcl
```
3. nom de la base de données, le nom de l'instance et la taille des blocs de la base de données
```SQL
SELECT database_name from v$database;
DATABASE_NAME
-------------------------------
ORCL
```
```SQL
select value from v$parameter where name = 'db_block_size';
8192
```
4. les noms et les tailles des fichiers de données
```SQL
select name,blocks from v$datafile;

/u01/app/oracle/oradata/orcl/system01.dbf      87040

/u01/app/oracle/oradata/orcl/sysaux01.dbf      67840

/u01/app/oracle/oradata/orcl/undotbs01.dbf     13440

/u01/app/oracle/oradata/orcl/users01.dbf       640

/u01/app/oracle/oradata/orcl/example01.dbf     12800
```

6. Le numéro de version 
```SQL
select version from v$instance;
VERSION
-----------------
11.2.0.1.0
```
7. le nombre de paramètrer(s)
```SQL
select value from v$parameter where name  ='processes';
150
```
8.
9. Modification de la taille des blocks
> Erreur
> DB_BLOCK_SIZE must be 8192 to mount this database (not 16384)

10. énumaration des paramètres d'initialisation par défaut.
```SQL
select * from dba_profiles where profile='DEFAULT';

PROFILE 		       RESOURCE_NAME			    RESOURCE    LIMIT
---------------------------------------------------------------------
DEFAULT 		       COMPOSITE_LIMIT			    KERNEL      UNLIMITED
DEFAULT 		       SESSIONS_PER_USER		    KERNEL      UNLIMITED
DEFAULT 		       CPU_PER_SESSION			    KERNEL      UNLIMITED
DEFAULT 		       CPU_PER_CALL			        KERNEL      UNLIMITED
DEFAULT 		       LOGICAL_READS_PER_SESSION	KERNEL      UNLIMITED
DEFAULT 		       LOGICAL_READS_PER_CALL		KERNEL      UNLIMITED
DEFAULT 		       IDLE_TIME			        KERNEL      UNLIMITED
DEFAULT 		       CONNECT_TIME			        KERNEL      UNLIMITED
DEFAULT 		       PRIVATE_SGA			        KERNEL      UNLIMITED
DEFAULT 		       FAILED_LOGIN_ATTEMPTS		PASSWORD    10
DEFAULT 		       PASSWORD_LIFE_TIME		    PASSWORD    180
DEFAULT 		       PASSWORD_REUSE_TIME		    PASSWORD    UNLIMITED
DEFAULT 		       PASSWORD_REUSE_MAX		    PASSWORD    UNLIMITED
DEFAULT 		       PASSWORD_VERIFY_FUNCTION 	PASSWORD    NULL
DEFAULT 		       PASSWORD_LOCK_TIME		    PASSWORD    1
DEFAULT 		       PASSWORD_GRACE_TIME		    PASSWORD    7
```
11. 
    * connection en tant que scott/tiger
    * Insertion dans la table EMP
    * Ouverture d'une deuxième session et arrête avec les mode
        - Immediate
        - Transaction
        - Normale
        - Abort
```SQL
connect scott/tiger
ou
sqlplus scott/tiger
```
```SQL
INSERT INTO EMP (EMPNO,ENAME,JOB,MGR,SAL,DEPTNO)VALUES(800,'DUPONT','ANALYST',7566,1500,20);
```
> avec le mode **Normale** : il y a un arrêt une fois que tous le autres utilisateurs se soient déconnectés
> avec le mode **Transaction** : il y a un arrêt une fois que toutes les transactions sont validées (dans notre exemple il y aura un arrêt après avoir fait un COMMIT de la requête d'insertion dans la table EMP) même s'il y a encore des utilisateurs connectés.
> avec le mode **Immediate** : il y a un arrêt immediate  
> avec le mode **Abort** : on force l'arrêt 

### Création d'une base de données

1. question 1,2,3 faire des mkdir dans les dossier correspondant
```
mkdir /u01/app/oracle/admin/myinst
mkdir /u01/app/oracle/oradata/myinst /u01/app/oracle/flash_recovery_data/myinst
mkdir /u01/app/oracle/admin/myinst/adump /u01/app/oracle/admin/myinst/dpdump /u01/app/oracle/admin/myinst/pfile
```
> copier coller le fichier initorcl165645.ora se trouvant dans /u01/app/oracle/admin/orcl/pfile
   dans le dossier /u01/app/oracle/admin/myinst/pfile puis le renomer initmyinst.ora.
remplacer toutes les occurences "orcl" se trouvant dans initmyinst.ora par "myinst"
commenter la ligne  :   **undo_tablespace=UNDOT.....**
ajouter la ligne :   **instance_name = myinst (en bas de db_name = myinst)**

5. Modifier le script crationBD.sql en comme indiqué dans le tp mais en laissant les tailles, ce qui donne le script suivant
```SQL
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
```

6. Démarrage de l'instance en mode nomount :
>  startup nomount pfile=/u01/app/oracle/admin/myinst/pfile/initmyinst.ora
puis éxecuter le script (ci-dessus) de création de la base de données 

7. monter le fichier
>  startup  pfile=/u01/app/oracle/admin/myinst/pfile/initmyinst.ora

verfication des fichiers de base de données : 
```SQL
select name,blocks from v$datafile;

> /u01/app/oracle/oradata/myinst/system01myinst.dbf
     12800

/u01/app/oracle/oradata/myinst/sysaux01.dbf
     41600

/u01/app/oracle/oradata/myinst/undotbs01.dbf
     25600
```
8. affichage des utilisateurs de la base de données :
```SQL
select * from all_users
```
> Erreur
#### Création des vues du dictionnaire de données et création des packages standard

1. Création des vues du dictionnaire de données
```SQL
start /u01/app/oracle/product/11.2.0.4/db_1/rdbms/admin/catalog.sql
```
2. Affichage des noms des utilisateur de la base de données
```SQL
select username from dba_users;
```
3. 
```SQL
start /u01/app/oracle/product/11.2.0.4/db_1/rdbms/admin/catproc.sql
```
4. Script de création de la base de données exemple
```SQL
start /u01/app/oracle/product/11.2.0.4/db_1/rdbms/admin/utlsampl.sql
```





