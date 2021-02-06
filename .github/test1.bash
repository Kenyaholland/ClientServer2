./testServer 8080 1 "We broke the security code!" &> /dev/null &
sleep .5

src/hackServer localhost 8080