-- Selection des images rouges
select NOM
from MULTIMEDIA
where COULEUR = 1
  and TAUXDEROUGE > 0.5;

-- Selection des images bleu
select NOM
from MULTIMEDIA
where COULEUR = 1
  and TAUXDEBLEU > 0.5;

-- Selection des images vert
select NOM
from MULTIMEDIA
where COULEUR = 1
  and TAUXDEVERT > 0.4;

-- Selection des images avec peu de contours
select NOM
from MULTIMEDIA
where CONTOUR < 0.00002;

-- Selection des images couleurs
select NOM
from MULTIMEDIA where COULEUR = 1;

-- Selection des images noir et blanc
select NOM
from MULTIMEDIA where COULEUR = 0;

declare
    nom_recherche  varchar2(50);
    nom_v          varchar2(50);
    distance_v     float;
    distace_tempo  float;
    ligne          multimedia%ROWTYPE;
    evaluated_elem multimedia%ROWTYPE;
    cursor m_cursor is select *
                       from multimedia;
    cursor m_cursor_oracle is select *
                              from multimedia;
    sig1           ordsys.ordimageSignature;

begin

    -- Calcul l'image avec la moyenne de gradient le plus proche
    -- Donc l'image avec le même taux de texture
    nom_recherche := '462.jpg';
    distance_v := 9999;
    nom_v := 'empty';
    select * into evaluated_elem from MULTIMEDIA where NOM = nom_recherche;
    for ligne in m_cursor
        loop
            distace_tempo := ABS(evaluated_elem.MOYENNEDUGRADIENT - ligne.MOYENNEDUGRADIENT);
            if distace_tempo < distance_v and ligne.NOM != evaluated_elem.NOM then
                distance_v := distace_tempo;
                nom_v := ligne.NOM;
            end if;
        end loop;
    DBMS_OUTPUT.PUT_LINE(
                'Closest element to ' || evaluated_elem.NOM || ' is ' || nom_v || ' with gradient custom descriptor');

    -- Calcul de l'image la plus proche avec les critères oracle
    distance_v := 9999;
    nom_v := 'empty';
    select * into evaluated_elem from MULTIMEDIA where NOM = nom_recherche;
    for ligne in m_cursor_oracle
        loop
            distace_tempo := ABS(ordsys.ordimageSignature.evaluateScore(evaluated_elem.SIGNATURE, ligne.SIGNATURE,
                                                                        'color = 0, texture = 1, shape =0, location = 0'));
            if distace_tempo < distance_v and ligne.NOM != evaluated_elem.NOM then
                distance_v := distace_tempo;
                nom_v := ligne.NOM;
            end if;
        end loop;
    DBMS_OUTPUT.PUT_LINE(
                'Closest element to ' || evaluated_elem.NOM || ' is ' || nom_v || ' with oracle descriptor');
end;