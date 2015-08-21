ls web_content/*.html > toCopy.log
scp web_content/*.html jmontejo@caronte.ifae.es:.
scp toCopy.log jmontejo@caronte.ifae.es:.
ssh jmontejo@caronte.ifae.es 'copyToAtropos.sh'
