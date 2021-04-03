set -eux

party_index=$1

valgrind --leak-check=full --show-leak-kinds=all ./SMMLF $party_index