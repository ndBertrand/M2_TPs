Bertrand NDAYISENGA
Master 2 Informatique
Systèmes d'information et d'aide à la décision
Année Académique : 2017-2018

## Administration des bases de données - TP

### Gestion des utilisateurs

1. Création de l'utilisateur bob avec comme tablespace par défaut DATA01
```SQL
CREATE USER bob IDENTIFIED BY along DEFAULT TABLESPACE DATA01;

User created
```

2 Création de l'utilisteur kay avec les droit nécessaire pour pouvoir se connecter et créer des objets.

> On peut aussi lui attribuer toutes les privilèges

```SQL
GRANT CONNECT TO kay IDENTIFIED BY mary;

Grant succeeded.

GRANT ALL PRIVILEGES TO kay;

Grant succeeded.
```

3. Affichage des informations(username, état du compte , tablespace) pour bob et kay.

```SQL
SELECT USERNAME,ACCOUNT_STATUS,DEFAULT_TABLESPACE FROM DBA_USERS WHERE USERNAME='BOB';

USERNAME		       ACCOUNT_STATUS
------------------------------ --------------------------------
DEFAULT_TABLESPACE
------------------------------
BOB			       OPEN
DATA01
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

c.


### Gestion des privilèges
