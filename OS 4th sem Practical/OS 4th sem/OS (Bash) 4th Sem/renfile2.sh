cd /mnt/e/Imp.Doc/'OS (Bash) 4th sem'

cd renfile

for i in *;
	do
	if [[ "$i" != *.sh ]]; then
	base="${i%.*}"
	mv "$i" "$base.sh"
	fi
	done
