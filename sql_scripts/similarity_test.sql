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
    sig1  ordsys.ordimageSignature;
    sig2  ordsys.ordimageSignature;
    sim   integer;
    dist  float;


begin
    -- Test de similarite entre 1 et 2
    select signature
    into sig1
    from multimedia
    where nom = '1.jpg';

    select signature
    into sig2
    from multimedia
    where nom = '2.jpg';


    sim := ordsys.ordimageSignature.isSimilar(sig1, sig2, 'color = 0.5, texture = 0, shape = 0, location = 0', 10);

    if sim = 1 then
        dbms_output.put_line('Images similaires');
    else
        dbms_output.put_line('Images non similaires');
    end if;

    -- calcul distance entre image1.jpg et image2.jpg
    dist := ordsys.ordimageSignature.evaluateScore(sig1, sig2, 'color = 0.5, texture = 0, shape =0, location = 0');
    dbms_output.put_line('Distance : ' || dist);


end ;

