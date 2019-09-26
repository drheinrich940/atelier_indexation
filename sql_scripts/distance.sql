drop table histogram_nb_table;

create table histogram_nb_table
(
    rowIdentifier integer,
    nom           varchar2(50),
    indiceNB      integer,
    tauxNB        float
);

declare
    nom_target varchar2(50) := '130.jpg';
    proximity  float        := 0.0;
    bestValue float := 99999.0;
    bestEquivalent varchar2(50);
    image_name varchar2(50);
    entry histogram_nb_table%ROWTYPE;
    cursor taux is select *
                   from histogram_nb_table
                   where nom = nom_target;
    cursor c_image is select *
                      from MULTIMEDIA
                      where nom != nom_target;
    currentValue histogram_nb_table%ROWTYPE;
begin
    for image_name in c_image
    loop
        for entry in taux
        loop
            select * into currentValue from histogram_nb_table where nom = image_name.NOM and indiceNB = entry.indiceNB;
            proximity := proximity + SQRT( currentValue.tauxNB * entry.tauxNB);
        end loop;
        if proximity >= 0 then
            --DBMS_OUTPUT.PUT_LINE('Current proximity is : ' || proximity);
             proximity := -LN(proximity);
            if proximity<bestValue then
                bestValue := proximity;
                bestEquivalent := image_name.NOM;
            end if;
        end if;
    end loop;
     DBMS_OUTPUT.PUT_LINE('Closest element to ' || nom_target || ' is ' || bestEquivalent);
end;