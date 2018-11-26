Bertrand NDAYISENGA
Master 2 Informatique
Systèmes d'information et d'aide à la décision
Année Académique : 2017-2018

## Administration des bases de données - TP

### Gestion des utilisateur

1.
```SQL
CREATE USER bob IDENTIFIED BY along DEFAULT TABLESPACE DATA01;

User created
```

2
GRANT CONNECT TO kay IDENTIFIED BY mary;

Grant succeeded.

GRANT ALL PRIVILEGES TO kay;

Grant succeeded.