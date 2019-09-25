drop table multimedia;

-- creation de la table d’images
create table multimedia
(
    nom       varchar2(50),
    image     ordsys.ordimage,
    signature ordsys.ordimageSignature
);

declare
    i     ordsys.ordimage;
    j     integer;
    ctx   RAW(400) := NULL;
    ligne multimedia%ROWTYPE;
    cursor mm is select *
                 from multimedia
                     for update;


begin

    -- Création des lignes dans la table des images
    for j in 1..4
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

--     -- Generation des signatures pour la table
    for ligne in mm
        loop
            ligne.signature.generateSignature(ligne.image);
            update multimedia
            set signature = ligne.signature
            where current of mm;
        end loop;
    commit;

end ;

