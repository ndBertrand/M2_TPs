Bertrand NDAYISENGA
Master 2 Informatique
Systèmes d'information et d'aide à la décision
Année Académique : 2017-2018

## Administration des bases de données - TP 4

### Gestion des utilisateurs

1. Création de l'utilisateur bob avec comme tablespace par défaut DATA01
```SQL
CREATE USER bob IDENTIFIED BY along DEFAULT TABLESPACE DATA01;

User created
```

2 
a . Création de l'utilisteur kay avec les droit nécessaire pour pouvoir se connecter et créer des objets.

```SQL
GRANT CONNECT TO kay IDENTIFIED BY mary;

Grant succeeded.

GRANT connect, create table TO  kay;

Grant succeeded.
```
a. Connection en tant que kay dans une nouvelle session
```sql
sqlplus kay/mary
```
b. copie de la able EMP à partir du schéma SCOTT dans le compte kay.
```sql
CREATE TABLE EMP AS SELECT  * FROM scott.emp;
```
`ERROR at line 1:
ORA-00942: table or view does not exist`
```sql
GRANT SELECT ON scott.emp to kay;
Grant succeeded.

create table emp as select * from scott.emp;
Table created.
```
>Pour y arriver on donne à kay le droit de faire un select sur la table EMP DE SCOTT.

3. Affichage des informations(username, état du compte , tablespace) pour bob et kay.

```SQL
SELECT USERNAME,ACCOUNT_STATUS,DEFAULT_TABLESPACE FROM DBA_USERS WHERE USERNAME='BOB' OR USERNAME='KAY';
USERNAME		  ACCOUNT_STATUS       DEFAULT_TABLESPACE
--------------------------------------------------------------
KAY			       OPEN                 USERS

BOB			       OPEN                 DATA01
```

5. Allocation d'un quota de 5M sur le tablespace DATA01.
```SQL
 ALTER USER kay QUOTA 5M ON DATA01;
User altered.
```
> Il faut éxecuter la commande suivante dans la session de kay

```SQL
SELECT * FROM USER_TS_QUOTAS;

TABLESPACE_NAME 	BYTES  MAX_BYTES	 BLOCKS     MAX_BLOCKS  DRO
-------------------------------------------------------------------
DATA01			    0      5242880	      0         640         NO
```

6. Suppression du quota de l'utilisateur kay sur son tablespace par défaut.

```SQL
 ALTER USER kay QUOTA 0K ON DATA01;
User altered.
```
> Il faut éxecuter la commande suivante dans la session de kay

```SQL
SELECT * FROM USER_TS_QUOTAS;

TABLESPACE_NAME 	BYTES  MAX_BYTES	 BLOCKS    MAX_BLOCKS   DRO
-------------------------------------------------------------------
DATA01			    0      0	         0         0            NO

```

7. Suppression en cascade du compte de kay

```SQL
DROP USER kay CASCADE;
User dropped.
```
> pour supprimer l'utilisateur kay, il faut le déconnecter avant. Sinon il y a le message d'erreur suivant `ERROR at line 1:
ORA-01940: cannot drop a user that is currently connected`

8. Mise à jour du mot de passe pour l'utilisateur Bob
```sql
ALTER USER bob IDENTIFIED BY olink;
User altered.
ALTER USER bob PASSWORD EXPIRE;
User altered.
```
9.
`connect bob/olink
ERROR:
ORA-28001: the password has expired`

>Changing password for bob
New password: 
Retype new password: 
`ERROR:
ORA-01045: user BOB lacks CREATE SESSION privilege; logon denied`
Password changed

> On constate que bob n'a pas le droit de se connecter ce qui est normale parcequ'on ne lui a pas en attribué ce droit, mais il a pu changer son mot de passe.

### Gestion des profils

1. 
```SQL
select * from dba_profiles;
```
PROFILE | RESOURCE_NAME | RESOURCE | LIMIT
------- | ------------- | -------- | -----
DEFAULT | COMPOSITE_LIMIT | KERNEL | UNLIMITED
DEFAULT | SESSIONS_PER_USER | KERNEL | UNLIMITED
DEFAULT | CPU_PER_SESSION | KERNEL | UNLIMITED
DEFAULT | CPU_PER_CALL | KERNEL | UNLIMITED
DEFAULT | LOGICAL_READS_PER_SESSION | KERNEL | UNLIMITED
DEFAULT | LOGICAL_READS_PER_CALL | KERNEL | UNLIMITED
DEFAULT | IDLE_TIME | KERNEL | UNLIMITED
DEFAULT | CONNECT_TIME | KERNEL | UNLIMITED
DEFAULT | PRIVATE_SGA | KERNEL | UNLIMITED
DEFAULT | FAILED_LOGIN_ATTEMPTS | PASSWORD | 10
DEFAULT | PASSWORD_LIFE_TIME | PASSWORD | 180
DEFAULT | PASSWORD_REUSE_TIME | PASSWORD | UNLIMITED
DEFAULT | PASSWORD_REUSE_MAX | PASSWORD | UNLIMITED
DEFAULT | PASSWORD_VERIFY_FUNCTION | PASSWORD | NULL
DEFAULT | PASSWORD_LOCK_TIME | PASSWORD | 1
DEFAULT | PASSWORD_GRACE_TIME | PASSWORD | 7



2. 
a. Création d'un nouveau profil de sorte que deux sessions par utilisateur soit permises et que le temps d'inactivité soit à une minute.

```SQL
CREATE PROFILE myprofile LIMIT
	SESSIONS_PER_USER	2
	CONNECT_TIME 		1;  
	
Profile created.
```
```SQL
SELECT * FROM DBA_PROFILES
WHERE PROFILE='MYPROFILE' AND RESOURCE_NAME IN ('SESSIONS_PER_USER','CONNECT_TIME');  

PROFILE 		       RESOURCE_NAME			RESOURCE    LIMIT
-----------------------------------------------------------------
MYPROFILE		       SESSIONS_PER_USER		KERNEL      2

MYPROFILE		       CONNECT_TIME			    KERNEL      1

ALTER SYSTEM SET RESOURCE_LIMIT = TRUE;

System altered.
````

b. 

```SQL
ALTER USER bob PROFILE MYPROFILE;
User altered.
SELECT USERNAME, PROFILE, ACCOUNT_STATUS FROM DBA_USERS WHERE USERNAME='BOB';
USERNAME		       PROFILE          ACCOUNT_STATUS
------------------------------------------------------
BOB			          MYPROFILE            OPEN
```

c.Ouverture de 3 sessions de l'utilisateur kay.
> on commence par donner à bob les droits de se créer une session
```SQL
GRANT CONNECT TO BOB;

Grant succeeded.
```
> en essayant d'ouvrire une 3ème session pour bob nous avons une erreur
```SQL
connect bob/olink
```
` 
ERROR:
ORA-02391: exceeded simultaneous SESSIONS_PER_USER limit`

### Gestion des privilèges

1. création de l'utilisateur kay
```SQL
CREATE USER kay IDENTIFIED BY mary;
User created.
GRANT CREATE TABLE TO kay;
Grant succeeded.
```

2. 
a. Connection en tant que SCOTT/tiger et octroi à kay des privilège SELECT SUR les tables SCOTT.EMP et SCOTT.dep

```SQL
connect scott/tiger
Connected.

GRANT SELECT ON scott.emp TO kay;
Grant succeeded.

 Grant select on scott.dept to kay;
Grant succeeded.
```

b. Connection en tant que kay et création des tables EMP et DEPT en copiant les données des tables correspondates scott.

```SQL
connect kay/mary
Connected.

CREATE TABLE kay.emp as SELECT * FROM scott.emp;
Table created.

CREATE TABLE kay.dept as SELECT * FROM scott.dept;
Table created.
```

c. Connection en tanque que SYSTEM/oracle, et octroi à bob la possibilité de séléctionner des données dans la table DEPT de kay;

```SQL
connect SYSTEM/oracle
Connected.

GRANT SELECT ON kay.dept TO bob;

Grant succeeded.
```
d. Octroi à bob le privilège de faire un select sur la table EMP de kay avec le droit d'administer ce privilège

```SQL
GRANT SELECT ON kay.emp TO bob WITH GRANT OPTION;
Grant succeeded.
```

3.
a. Examiner les vues du dictionnaire de données qui enregistre ces informations.
```SQL
SELECT * FROM dba_users;
```

4. Création de l'utilisateur todd avec la possibilité de se connecter à la base de données.
```SQL
GRANT CONNECT TO todd IDENTIFIED by pswd;
Grant succeeded.
```
5. 
a .Activation de l'accès de todd à la table EMP de kay en tant que bob
```SQL
connect bob/olink
Connected.

GRANT SELECT ON kay.emp TO todd;
Grant succeeded.
```
Vérification de l'opération
```SQL
SELECT GRANTEE,TABLE_NAME,PRIVILEGE,OWNER FROM dba_tab_privs WHERE OWNER='KAY' AND GRANTEE='TODD';
GRANTEE 		       TABLE_NAME       PRIVILEGE				 OWNER
-----------------------------------------------------------------------
TODD			       EMP              SELECT					 KAY
```

b. En tant que kay suppression du privilège de lecture de bob sur ses tables
```SQL
connect kay/mary
Connected.

REVOKE SELECT ON kay.emp FROM bob;
Revoke succeeded.
```

c. Interrogation de la table EMP de kay en tant que todd

```SQL
connect todd/pswd
Connected.
```
> il y a une erreur la table kay.emp n'est plus accessible à partir de la session de todd
`ORA-00942: table or view does not exist`

6.
a. Pour kay, activez la création de tables dans un schéma quelconque.En tant que kay, créez la table EMP dans le schéma de bob à partir de KAY.EMP. 
```SQL
CONNECT / as sysdba
Connected.

GRANT CREATE ANY TABLE TO kay;

Grant succeeded.
```

```SQL
connect kay/mary
Connected.
ALTER USER bob QUOTA 5M ON DATA01;
CREATE TABLE bob.emp as select * from kay.emp;
Table created.
```
>Pour pouvoir créer une table dans le schéma bob il faut lui donner d'abord un quotas.

b.

7. Pour kay, activez le démarrage et l’arrêt de la base de données sans la possibilité d’en créer une nouvelle. 
```SQL
GRANT sysoper TO kay;
Grant succeeded.
```

### Gestion des rôles
1. 

```SQL
select * from dba_sys_privs WHERE GRANTEE='RESOURCE';

GRANTEE 		       PRIVILEGE				ADM
----------------------------------------------------
RESOURCE		       CREATE TRIGGER				NO
RESOURCE		       CREATE SEQUENCE				NO
RESOURCE		       CREATE TYPE				NO
RESOURCE		       CREATE PROCEDURE 			NO
RESOURCE		       CREATE CLUSTER				NO
RESOURCE		       CREATE OPERATOR				NO
RESOURCE		       CREATE INDEXTYPE 			NO
RESOURCE		       CREATE TABLE				NO
```
2. Créez un rôle DEV permettant de créer une table, une vue et de sélectionner les données dans la table EMP de kay. 

```sql
CREATE ROLE Dev;

Role created.

GRANT CREATE TABLE TO Dev;   

Grant succeeded.

GRANT CREATE VIEW To Dev;
 
Grant succeeded.

GRANT SELECT ON kay.emp To Dev;

Grant succeeded.

```
3.
Vérification des privilèges  systèmes du rôle Dev
```SQL
SELECT GRANTEE,PRIVILEGE FROM DBA_ROLES  , DBA_SYS_PRIVS  B where DBA_SYS_PRIVS.GRANTEE = DBA_ROLES.GRANTEE; 
```

5. Allouez les rôles RESOURCE et DEV à bob. Déterminez l’activation automatique du rôle DEV quand bob se connecte.
```SQL
GRANT RESOURCE,DEV To bob;

Grant succeeded.
```
b. Accordez à bob la possibilité de lire toutes les informations du dictionnaire de données.

```SQL
 GRANT SELECT ANY DICTIONARY TO bob;

Grant succeeded.
```

6. bob doit vérifier les rollback segments actuellement utilisés par l’instance. Connectez vous en
tant que bob et énumérez les rollback segments utilisés.

```SQL
connect bob/olink
Connected.

SELECT t1.name, t2.extents, t2.rssize, t2.optsize, t2.hwmsize, t2.xacts, t2.status
FROM v$rollname t1, v$rollstat t2
WHERE t2.usn = t1.usn;
```


