Bertrand NDAYISENGA
Master 2 Informatique
Systèmes d'information et d'aide à la décision
Année Académique : 2017-2018

## Administration des bases de données - TP2
### Utilisation des outils d'adminstartion mode ligne
1. Connection en tant que SYS à l'instance oracle myinst. 
```sql
sqlplus / as sysdba
startup pfile=/u01/app/oracle/admin/myinst/pfile/initmyinst.ora
show parameter instance_name
NAME				     TYPE	 VALUE
------------------------------------ ----------- ------------------------------
instance_name			string	 myinst
```

2. Taille du buffer cache de données.
```sql
SELECT * FROM v$sga;
NAME			        VALUE
-------------------- ----------
Fixed Size		        2213736
Variable Size	        922749080
Database Buffers        654311424
Redo Buffers		    7434240
```

3. Taille du SGA
> La taille du SGA est la somme de toutes les tailles.
```sql
SELECT SUM(value) from v$sga;
SUM(VALUE)
----------
1586708480

```

4. Lister les colonnes, **owner**, **table_name**, **tablespace_name** de la vue **dba_tables** du dictinnaire de données pour les tables et les tables et tablespaces dont le propriétaire est l'utilsateur  SCOTT.

```SQL
select owner,table_name, tablespace_name from dba_tables where owner='SCOTT';

OWNER			       TABLE_NAME       TABLESPACE_NAME
-------------------------------------------------------
SCOTT			       DEPT             USERS
SCOTT			       EMP              USERS
SCOTT			       BONUS            USERS
SCOTT			       SALGRADE         USERS
```

`NE PAS FAIRE LES QUESTIONS 5,6,7 ET 8`

### Mise à jour du ficher de contrôle
1. Sauvegarde du fichier de contrôle
```sql
ALTER DATABASE BACKUP CONTROLFILE TO TRACE;
Database altered.
```
> Cette commande crée un fichier texte dans le répertoire **USER_DUMP_DEST**, qu'on peut modifier pour reconstruire un nouveau fichier de contrôle.

2. Vérification de création des fichiers de contrôle. 
> En regardant le fichier le fichier **alert_orcl.log** se trouvant **/u01/app/oracle/diag/rdbms/orcl/orcl/trace** à la fin de fichier, l'emplacement des sauvegardes est marqué.
```txt
Fri Oct 26 16:20:13 2018
ALTER DATABASE BACKUP CONTROLFILE TO TRACE
Backup controlfile written to trace file /u01/app/oracle/diag/rdbms/orcl/orcl/trace/orcl_ora_2990.trc
```
3. Emplacement des souvegarde.
```sql
show parameter control_files

NAME				     TYPE	 VALUE
------------------------------------ ----------- ------------------------------
control_files			string	 /u01/app/oracle/oradata/myinst/control01.ctl
```
4. Démarrage sans fichier de contôle
```sql
startup pfile=/u01/app/oracle/admin/orcl/pfile/init2.ora
ORA-00205: error in identifying control file, check alert log for more info
```
> Il a une erreur lors de l'ouverture de la base de données.


5. Multiplexage du ficher de controle

> si on a 2 ou plusieurs fichiers de controle, ce sont les même(ce sont des copie )
 * faire un shutdown immediate(obligatoire pour ne pas avoir de problème)
 * modifier le fichier de paramètre(le fichier init) 
 * décommenter la ligne avec **controle_files**
 * on le ajoute un autre fichier de contolre ("--------","---------","---------)
 *ensuite faire la copier du fichier de controle qlq part sur le disk comme $HOME/DISK2
 * après on redemare 
Vérification que le fichier de contrôle a bien été crée.
```SQL
    startup pfile=/u01/app/oracle/admin/myinst/pfile/initmyinst2.ora
 select name from v$controlfile;
```

6. Le nombre maximum de fichiers de données que l'on puisse créer dans la base de données aisn que le le nombre actuell de fichiers de données dans la base de données. 
```sql
select records_total, records_used from v$controlfile_record_section where type='DATAFILE';
 RECORDS_TOTAL RECORDS_USED
------------- ------------
	    30		 7
```
> Comme spécifier dans le script de création de la bse de données du TP1, le nombre maximum de ficher que l'on puisse créer est égale à **30**

### Mise à jour des fichiers de reprise

1. Enumération du nombre et de l'emplacement des fichiers de reprise existants.
```SQL
 select member from v$logfile;

MEMBER
--------------------------------------------------------------------------------
/u01/app/oracle/oradata/orcl/redo03.log
/u01/app/oracle/oradata/orcl/redo02.log
/u01/app/oracle/oradata/orcl/redo01.log
```
2. mode d'archivage
> le mode d'archivage n'est pas activé
```SQL
select log_mode from v$database;
LOG_MODE
------------
NOARCHIVELOG
```
3. le groupe courant
> le groupe courant est le groupe 2
```SQL
select * from v$log where status = 'CURRENT';

GROUP#    THREAD#  SEQUENCE#      BYTES  BLOCKSIZE	  MEMBERS ARC
---------- ---------- ---------- ---------- ---------- ---------- ---
STATUS		 FIRST_CHANGE# FIRST_TIM NEXT_CHANGE# NEXT_TIME
---------------- ------------- --------- ------------ ---------
	 2	    1	      14   52428800	   512		1 NO
CURRENT 	       1205703 26-OCT-18   2.8147E+14
```

4. Modification du groupe courant 
```SQL
ALTER SYSTEM SWITCH LOGFILE ; 
System altered.
```

```SQL
select * from v$log where status = 'CURRENT';
GROUP#    THREAD#  SEQUENCE#      BYTES  BLOCKSIZE	  MEMBERS ARC
---------- ---------- ---------- ---------- ---------- ---------- ---
STATUS		 FIRST_CHANGE# FIRST_TIM NEXT_CHANGE# NEXT_TIME
---------------- ------------- --------- ------------ ---------
	 3	    1	      15   52428800	   512		1 NO
CURRENT 	       1215552 26-OCT-18   2.8147E+14
```
> On constate que effectivement, le groupe courrant est passé de 2 à 3

5.  Ajout d'un membre de reprise à chaque Groupe
```SQL
ALTER DATABASE ADD LOGFILE MEMBER '/u01/app/oracle/oradata/orcl/redo01a.log' to GROUP 1;
Database altered.
ALTER DATABASE ADD LOGFILE MEMBER '/u01/app/oracle/oradata/orcl/redo02a.log' to GROUP 2;     
Database altered.
ALTER DATABASE ADD LOGFILE MEMBER '/u01/app/oracle/oradata/orcl/redo03a.log' to GROUP 3;    
Database altered.
```
Verification
```SQL
select members from v$log;
MEMBERS
----------
	 2
	 2
	 2

```
> on constate que chaque groupe à maintenant 2fichiers de de reprises
> les fichiers  redo1a.log, redo2a.log et redoa3.log ont été crée dans le répertoire /u01/app/oracle/oradata/orcl

6. Création d'un nouveau groupe de reprise dans le répertoire DISK4 
```SQL
alter database add logfile ('/home/oracle/adminbd/DISK4/gzlog.rdo') size 4m;

Database altered.
```
vérification
```SQL
select count(*) from v$log;
  COUNT(*)
----------
	 4
```