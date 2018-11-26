Bertrand NDAYISENGA
Master 2 Informatique
Systèmes d'information et d'aide à la décision
Année Académique : 2017-2018

## Administration des bases de données - TP3
### Gestion des fichiers de données et des tablespaces

`
Ne pas créer la tablespace RFS, ne pas faire les question 7,8,16 pour la partie Gestion des fichiers de données et des tablespaces et les question 4 et 5 pour  la partie segement,
`

3. Création des tables space : 
```sql
CREATE TABLESPACE DATA01 DATAFILE '/u01/app/oracle/oradata/myinst/Disk4/data01' size 2m 
EXTENT MANAGEMENT LOCAL AUTOALLOCATE SEGMENT SPACE MANAGEMENT AUTO;

CREATE TABLESPACE TEMP DATAFILE '/u01/app/oracle/oradata/myinst/Disk3/temp02' size 5m 
EXTENT MANAGEMENT LOCAL AUTOALLOCATE SEGMENT SPACE MANAGEMENT AUTO;

CREATE TABLESPACE INDX01 DATAFILE '/u01/app/oracle/oradata/myinst/Disk3/indx01' size 3m 
EXTENT MANAGEMENT LOCAL AUTOALLOCATE SEGMENT SPACE MANAGEMENT AUTO;


CREATE TABLESPACE RONLY DATAFILE '/u01/app/oracle/oradata/myinst/Disk1/ronly01' size 2m 
EXTENT MANAGEMENT LOCAL AUTOALLOCATE SEGMENT SPACE MANAGEMENT AUTO;

ALTER TABLESPACE RONLY READ ONLY;
```

4. la liste des tablespaces par défaut
```sql
  select username, default_tablespace from dba_users;

USERNAME		       DEFAULT_TABLESPACE
------------------------------ ------------------------------
SYSTEM			       SYSTEM
SYS			           SYSTEM
OUTLN			       SYSTEM
DBSNMP			       SYSAUX
APPQOSSYS		       SYSAUX
```


```sql
SELECT TABLESPACE_NAME FROM DBA_DATA_FILES;

TABLESPACE_NAME
------------------------------
SYSTEM
SYSAUX
UNDOTBS
DATA01
INDX01
RONLY
DATA01
```

5. Mettre le tablespace DATA01 comme tablespace par défaut.
```sql
ALTER DATABASE default TABLESPACE DATA01;
Database altered

select username, default_tablespace from dba_users;

USERNAME		       DEFAULT_TABLESPACE
------------------------------ ------------------------------
SYSTEM			       SYSTEM
SYS			           SYSTEM
OUTLN			       SYSTEM
DBSNMP			       SYSAUX
APPQOSSYS		       SYSAUX
ORACLE_OCM		       DATA01
DIP			           DATA01
SCOTT			       DATA01
```
6. Liste des tablespaces temporaires par défaut
```sql
select file_name,tablespace from dba_temp_files;

USERNAME		       DEFAULT_TABLESPACE
------------------------------ ------------------------------
SYSTEM			       SYSTEM
SYS			           SYSTEM
OUTLN			       SYSTEM
DBSNMP			       SYSAUX
APPQOSSYS		       SYSAUX
ORACLE_OCM		       DATA01
DIP			           DATA01
SCOTT			       DATA01
```

9. 
> il y a 2 solution pour augmenter la taille des tablespace : 
    - soit on ajoute des fichiers au table space
    -soit on choisi un fichier et on augmente sa taille.

```sql
ALTER DATABASE DATAFILE '/u01/app/oracle/oradata/myinst/Disk4/data01' resize 5m
ALTER TABLESPACE DATA01 ADD DATAFILE '/u01/app/oracle/oradata/myinst/Disk4/data02' size 2m 
```

10. déplacer le tablespace INDX01 vers DISK1
> On vérifie l'emplacement du datafile INDX01 (dans notre cas il se trouve dans le DISK3)
```SQL
select FILE_NAME, TABLESPACE_NAME from  DBA_DATA_FILES WHERE TABLESPACE_NAME = 'INDX01';
FILE_NAME                                       TABLESPACE_NAME
-----------------------------------------------------------------
/u01/app/oracle/oradata/myinst/Disk3/indx01     INDX01

```
> On déplace physiquement l'emplacement du fichier
```shell
mv /u01/app/oracle/oradata/myinst/Disk3/indx01  /u01/app/oracle/oradata/myinst/Disk1/
```
> On met à jour le tablespace
```SQL
 ALTER TABLESPACE INDX01 OFFLINE;
Tablespace altered.
ALTER TABLESPACE INDX01 RENAME DATAFILE '/u01/app/oracle/oradata/myinst/Disk3/indx01' TO '/u01/app/oracle/oradata/myinst/Disk1/indx01';
Tablespace altered.
ALTER TABLESPACE INDX01 ONLINE;
Tablespace altered.
```
Vérification
```SQL
select FILE_NAME, TABLESPACE_NAME from  DBA_DATA_FILES WHERE TABLESPACE_NAME = 'INDX01';
FILE_NAME                                       TABLESPACE_NAME
-----------------------------------------------------------------
/u01/app/oracle/oradata/myinst/Disk1/indx01     INDX01
```

11. Mettre la tablespace RONLY EN LECTURE seule après y avoir créer une table
```SQL
ALTER TABLESPACE RONLY READ WRITE;
Tablespace altered.
CREATE TABLE subjects (
	id    NUMBER(5) PRIMARY KEY,
  	name  VARCHAR(10) NOT NULL
  	);
TABLESPACE RONLY;

ALTER TABLESPACE RONLY READ ONLY;
```


12. Supprimez le tablespace RONLY et vérifiez l’opération. 
```SQL
DROP TABLESPACE RONLY INCLUDING CONTENTS; 
Tablespace dropped.
```
vérification 
```SQL
select tablespace_name from dba_tablespaces;

TABLESPACE_NAME
------------------------------
SYSTEM
SYSAUX
UNDOTBS
TEMPTS1
DATA01
TEMP
INDX01
```

13. Afficher la liste des tablespaces, les fichiers associés et la taille de chaque fichier 

```SQL
SELECT TABLESPACE_NAME,FILE_NAME, BLOCKS,  FROM DBA_DATA_FILES;
```
TABLESPACE_NAME | FILE_NAME | BLOCKS | 
--------------- |---------- | ------ |
SYSTEM | /u01/app/oracle/oradata/myinst/system01myinst.dbf | 25600 
SYSAUX | /u01/app/oracle/oradata/myinst/sysaux01.dbf | 41600 
UNDOTBS | /u01/app/oracle/oradata/myinst/undotbs01.dbf | 25600 
DATA01 | /u01/app/oracle/oradata/myinst/Disk4/data01 | 640
INDX01 | /u01/app/oracle/oradata/myinst/Disk1/indx01 | 2560 
DATA01 | /u01/app/oracle/oradata/myinst/Disk4/data02 | 256

15. Afficher la taille de l’espace libre dans chaque tablespace 
```SQL
SELECT TABLESPACE_NAME , 
   max(BLOCKS) AS MAXIMUM,
   min(BLOCKS) AS MINIMUM,
   sum(BLOCKS) AS TOTAL
   FROM DBA_FREE_SPACE
GROUP BY TABLESPACE_NAME;

TABLESPACE_NAME 		MAXIMUM     MINIMUM	    TOTAL
-----------------------------------------------------
SYSAUX				    32984       32984	    32984
UNDOTBS 			    7040	    8	        17480
SYSTEM				    40	        40	        40
DATA01				    512	        128	        640
INDX01				    2432	    2432	    2432
```
