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
where CONTOUR < 500;


declare
    nom_v          varchar2(50);
    distance_v      float;
    distace_tempo  float;
    ligne          multimedia%ROWTYPE;
    evaluated_elem multimedia%ROWTYPE;
    cursor m_cursor is select *
                       from multimedia;
begin

    -- Calcul l'image avec la moyenne de gradient le plus proche
    -- Donc l'image avec le même taux de texture
    distance_v := 9999;
    nom_v := 'empty';
    select * into evaluated_elem from MULTIMEDIA where NOM = '462.jpg';
    for ligne in m_cursor
        loop
            distace_tempo := ABS(evaluated_elem.MOYENNEDUGRADIENT - ligne.MOYENNEDUGRADIENT);
            if distace_tempo < distance_v and ligne.NOM != evaluated_elem.NOM  then
                distance_v := distace_tempo;
                nom_v := ligne.NOM;
            end if;
        end loop;
    DBMS_OUTPUT.PUT_LINE('Closest element to ' || evaluated_elem.NOM || ' is ' || nom_v);
end;