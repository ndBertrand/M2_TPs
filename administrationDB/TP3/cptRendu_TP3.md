Bertrand NDAYISENGA
Master 2 Informatique
Systèmes d'information et d'aide à la décision
Année Académique : 2017-2018



# Administration des bases de données - TP3
## Gestion des fichiers de données et des tablespaces



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
SYS			       SYSTEM
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
SYS			       SYSTEM
OUTLN			       SYSTEM
DBSNMP			       SYSAUX
APPQOSSYS		       SYSAUX
ORACLE_OCM		       DATA01
DIP			       DATA01
SCOTT			       DATA01
```
6. Liste des tablespaces temporaires par défaut
```sql
select file_name,tablespace from dba_temp_files;

USERNAME		       DEFAULT_TABLESPACE
------------------------------ ------------------------------
SYSTEM			       SYSTEM
SYS			       SYSTEM
OUTLN			       SYSTEM
DBSNMP			       SYSAUX
APPQOSSYS		       SYSAUX
ORACLE_OCM		       DATA01
DIP			       DATA01
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




