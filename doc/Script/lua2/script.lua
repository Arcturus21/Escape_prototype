function Vector3:print()
	print("redéfinition de la méthode print de Vector3");
	print(self:length());
end

function Vector3:printLength()
	print("norme du vecteur : "..self:length());
end

print("debut du script lua");

v = Vector3();
u = Vector3(10,20,30);
v:print();
u:print();
print(u:length());
print("addition :");
u:add(Vector3(10,10,10)):print();
print("fin du script lua");

