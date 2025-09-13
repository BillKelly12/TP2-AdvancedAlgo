#!/bin/bash
set -euo pipefail

# Paramètres
N=10
M=4
FICHIER="benef.dat"

rm -f "$FICHIER"

# Écrire un entier 32 bits little-endian
write_le_int() {
  local v=$1
  printf "%08x" "$v" | sed 's/\(..\)\(..\)\(..\)\(..\)/\4\3\2\1/' | xxd -r -p >> "$FICHIER"
}

# Écrit N et M
write_le_int "$N"
write_le_int "$M"

# Générer les bénéfices avec log+ceil simplifié
# Ici on utilise awk pour calculer ceil(log(n)) facilement
for ((m=0; m<M; m++)); do
  for ((n=0; n<=N; n++)); do
    if (( n == 0 )); then
      val=0
    else
      ceil_log=$(awk -v n="$n" 'BEGIN { print int(log(n))+((log(n)>int(log(n)))?1:0) }')
      val=$(( n * ceil_log + (m*m)%17 ))
    fi
    write_le_int "$val"
  done
done

echo "Fichier binaire généré : $FICHIER (N=$N, M=$M, $((M*(N+1))) valeurs)"
