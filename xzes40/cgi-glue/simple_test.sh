curl \
  --include \
  --request POST \
  --header "Content-Type: multipart/form-data" \
  --form "xml=@/home/vagrant/xzes40/xzes40/transformer/examples/simple.xml" \
  --form "xsl=@/home/vagrant/xzes40/xzes40/transformer/examples/simple.xsl" \
  localhost:8080/xzes.py
