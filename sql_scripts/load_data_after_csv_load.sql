drop table distance_table;

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

    -- Initialisation des images et signature
    for ligne in mm
        loop
            update MULTIMEDIA
            set IMAGE     = ordsys.ordimage.init(),
                SIGNATURE = ordsys.ordimageSignature.init()
            where nom = ligne.NOM;

        end loop;

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

