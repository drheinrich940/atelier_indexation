--drop table multimedia;
--drop table distance_table;

create type histogram as VARRAY (256) of double precision;

-- creation de la table d’images
create table multimedia
(
    nom               varchar2(50),
    image             ordsys.ordimage,
    signature         ordsys.ordimageSignature,
    couleur           integer,
    Contour           integer,
    TauxDeRouge       float,
    TauxDeVert        float,
    TauxDeBleu        float,
    MoyenneDuGradient float,
    Histogramme       histogram
);

create table distance_table
(
    fisrt_image_name  integer,
    second_image_name integer,
    distance          float
);

declare
    i                ordsys.ordimage;
    j                integer;
    ctx              RAW(400) := NULL;
    ligne            multimedia%ROWTYPE;
    cursor mm is select *
                 from multimedia
                     for update;
    ligne_distance   multimedia%ROWTYPE;
    colonne_distance multimedia%ROWTYPE;
    cursor mm_row is select *
                     from multimedia;
    cursor mm_col is select *
                     from multimedia;


begin

    -- Création des lignes dans la table des images
    for j in 1..20
        loop
            insert into multimedia(nom, image, signature)
            values (j || '.jpg', ordsys.ordimage.init(), ordsys.ordimageSignature.init());
        end loop;
    commit;

    -- Ajout du contenu image
    for ligne in mm
        loop
            select ligne.image
            into i
            from multimedia
            where nom = ligne.nom for
                update;
            i.importFrom(ctx, 'file', 'IMG', ligne.nom);
            update multimedia
            set image = i
            where nom = ligne.nom;
        end loop;
    commit;

    -- Generation des signatures pour la table
    for ligne in mm
        loop
            ligne.signature.generateSignature(ligne.image);
            update multimedia
            set signature = ligne.signature
            where current of mm;
        end loop;
    commit;

    -- Generation de la matrice de distance
    for ligne_distance in mm_row
        loop
            for colonne_distance in mm_col
                loop
                    insert into distance_table(fisrt_image_name, second_image_name, distance)
                    values (CAST(REPLACE(ligne_distance.NOM, '.jpg', '') AS INT),
                            CAST(REPLACE(colonne_distance.NOM, '.jpg', '') AS INT),
                            ordsys.ordimageSignature.evaluateScore(ligne_distance.SIGNATURE, colonne_distance.SIGNATURE,
                                                                   'color = 0.5, texture = 0, shape =0, location = 0'));
                end loop;
        end loop;
    commit;

end ;