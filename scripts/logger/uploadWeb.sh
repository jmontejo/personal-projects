scp ~/scripts/logger/web_content/*.html jmontejo@caronte.ifae.es:. &> /dev/null
ssh jmontejo@caronte.ifae.es './uploadWeb.sh'
