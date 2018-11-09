Bertrand NDAYISENGA
Master 2 Informatique
Systèmes d'information et d'aide à la décision
Année Académique : 2017-2018

# Administration des bases de données - TP2

## Utilisation des outils d'adminstartion mode ligne

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

4. Lister les colonnes, **owner**, **table_name**, **tablespace_name** de la vue **dba_tables** du dictinnaire de données pour les tables et les tables et tablespaces dont le propriétaire est l'utilsateur  SCOTT.


## Mise à jour du ficher de contrôle
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

6. Le nombre maximum de fichiers de données que l'on puisse créer dans la base de données aisn que le le nombre actuell de fichiers de données dans la base de données. 
```sql
select records_total, records_used from v$controlfile_record_section where type='DATAFILE';
 RECORDS_TOTAL RECORDS_USED
------------- ------------
	    30		 7
```
> Comme spécifier dans le script de création de la bse de données du TP1, le nombre maximum de ficher que l'on puisse créer est égale à **30**


