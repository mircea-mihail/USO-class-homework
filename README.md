# USO-class-homework
Am lucrat la 2 proiecte pentru cursul de Utilizarea Sistemelor de Operare. Ambele au fost realizate codand in C. Ele sunt aplicatii de terminal.

## Primul proiect
Codul verifica daca un fisier este contiguu altuia. Altfel spus, verifca daca continutul primului se regaseste in al doilea si de cate ori o face. Pentru a face acest lucru, codul compara caracter cu caracter continutul fisierelor pana gaseste ceva care se verifica.

Drept exemplu, daca in fisierul din care caut contine "ab" si cel in care caut contine "caabab", comparatile efectuate de cod sunt:
  - a != c    (**a**b vs **c**aabab)
  - a == a
  - b != a
  - a == a    (urmatorul a din "ca**a**bab")
  - b == b    (primul match realizat)
  - a != b    (ne aflam la caa**b**ab)
  - a == a
  - b == b    (al doilea si ultimul match)
  - b != b    (si am terminat de verificat fisierul)
  
Aplicatia se apeleaza in felul urmator:
```  
$contiguu fisier_cu_textul_cautat fisier_in_care_doresc_sa_gasesc_textul
```
