# teorema chino del resto
#Si un numero X cumple:
#X ≡ m (mod p)
#X ≡ m (mod q)
#
#Entonces:
#
#X ≡ m (mod p*q)
#
#Aplicado a RSA:
#
#m^(e*d) ≡ m (mod n)


#   e * d ≡ 1 (mod phi(n))
#   m^phi(n) ≡ 1 (mod n) si mcd(m,n) = 1

# p y q son primos arbitrarios, en la practica de 256 bytes c/u   
p = 5
q = 11
n = p*q
phi=(p-1)*(q-1) # cantidad de coprimos que existen entre 1 y n; a n.

e = 3
d = 27

# phi = 40, n = 50
euler = pow(3,phi) % n # m^phi(n) ≡ 1 (mod n) = 1 si mcd(m,n) = 1

test = pow(3,e*d) % n
test2 = pow(3,(1+phi)) % n # test y test2 son equivalentes porque : e * d = 1 + k * phi(n); obviamente k es una constante iterativa

testAlgo = (e*d)%phi # = 1 , es decir E es inverso modular de D mod phi

print(test) 

