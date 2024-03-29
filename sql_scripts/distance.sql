drop table histogram_nb_table;

create table histogram_nb_table
(
    rowIdentifier integer,
    nom           varchar2(50),
    indiceNB      integer,
    tauxNB        float
);

declare
    nom_target     varchar2(50) := '130.jpg';
    proximity      float        := 0.0;
    bestValue      float        := 99999.0;
    bestEquivalent varchar2(50);
    image_name     varchar2(50);
    entry          histogram_nb_table%ROWTYPE;
    cursor taux is select *
                   from histogram_nb_table
                   where nom = nom_target;
    cursor c_image is select *
                      from MULTIMEDIA
                      where nom != nom_target;
    currentValue   histogram_nb_table%ROWTYPE;
begin
    for image_name in c_image
        loop
            for entry in taux
                loop
                    select *
                    into currentValue
                    from histogram_nb_table
                    where nom = image_name.NOM
                      and indiceNB = entry.indiceNB;
                    proximity := proximity + SQRT(currentValue.tauxNB * entry.tauxNB);
                end loop;
            if proximity >= 0 then
                --DBMS_OUTPUT.PUT_LINE('Current proximity is : ' || proximity);
                proximity := -LN(proximity);
                if proximity < bestValue then
                    bestValue := proximity;
                    bestEquivalent := image_name.NOM;
                end if;
            end if;
        end loop;
    DBMS_OUTPUT.PUT_LINE('Closest element to ' || nom_target || ' is ' || bestEquivalent);
end;

create table dist_batacharya_table
(
    image1   varchar2(50),
    image2   varchar2(50),
    distance float
);



SELECT image2, distance FROM dist_batacharya_table WHERE distance<3 AND image2 != image1 AND image1 = '462.jpg' AND ROWNUM <= 100 ORDER BY distance;

/*
SELECT image2 FROM dist_batacharya_table
INNER JOIN
    (
        SELECT image2, MIN(dist_batacharya_table.distance) minDist
        FROM dist_batacharya_table
        GROUP BY distance
        )tab ON tab.image2 = dist_batacharya_table.image2
WHERE distance = minDist AND dist_batacharya_table.image2 = '1.jpg';
*/