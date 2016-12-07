print("debut du script lua");
var = 10;
toto = "bonjour"
tableau = {"a","b","c","d"};
helloWorld();
affiche("mon message");
printParams(1,2,3,"message",true,false);
printParams("a","b","c",12.5,128);

print("retour de halfParam(10) : "..halfParam(10));
print("retour de halfParam(12.5) : "..halfParam(12.5));
print("retour de halfParam(128) : "..halfParam(128));


print("fin du script lua");

function luaPrint(m)
	print(m);
end

function addition(a,b)
	print("addition : "..(a+b));
	return a+b;
end