n = 811
p = 929

mod = p*n

fun = ( n - 1 ) * ( p - 1 )

d = 0

for i in range(2,fun): #mcd 
    if (fun %i == 1):
        d = i
        break
    
e = 0

for i in range(2, fun):
    if (i * d)%fun == 1:
        e = i
        break 

print(f"p={p} y n={n} primos")
print("d =",d)
print("e =",e)

print(f"clave privada: ({mod},{d})")
print(f"clave publica: ({mod},{e})")


mensaje = 2

cifrado = pow(mensaje,e)%mod

print(f"cifrado {cifrado}")

decifrado = pow(cifrado,d)%mod

print(f"descifrado {decifrado}")


print(f"Decifrado con doble exponente {pow(mensaje,e*d)%mod}")