#!/bin/bash

echo "Image: garden(500 × 500) with time step 10"
./CONVERT images/garden.jpg audio/gardenSerial10.wav 10 
./CONVERTTBB images/garden.jpg audio/gardenTBB10.wav 10 
./CONVERTTBBISPC images/garden.jpg audio/gardenTBBISPC10.wav 10

echo "Image: magic(1000 × 1000) with time step 10"
./CONVERT images/magic.jpeg audio/magicSerial10.wav 10 
./CONVERTTBB images/magic.jpeg audio/magicTBB10.wav 10 
./CONVERTTBBISPC images/magic.jpeg audio/magicTBBISPC10.wav 10

echo "Image: reef(1000 × 1500) with time step 10"
./CONVERT images/reef.jpeg audio/reefSerial10.wav 10 
./CONVERTTBB images/reef.jpeg audio/reefTBB10.wav 10 
./CONVERTTBBISPC images/reef.jpeg audio/reefTBBISPC10.wav 10

echo "Image: moon(1500 × 1000) with time step 10"
./CONVERT images/moon.jpg audio/moonSerial10.wav 10 
./CONVERTTBB images/moon.jpg audio/moonTBB10.wav 10 
./CONVERTTBBISPC images/moon.jpg audio/moonTBBISPC10.wav 10

echo "Image: universe(2000 × 2000) with time step 10"
./CONVERT images/universe.jpg audio/universeSerial10.wav 10 
./CONVERTTBB images/universe.jpg audio/universeTBB10.wav 10 
./CONVERTTBBISPC images/universe.jpg audio/universeTBBISPC10.wav 10


# echo "Image: snowflake(640 × 640) time step 10"
# ./CONVERT images/snowflake.jpg audio/snowflakeSerial10.wav 10 
# ./CONVERTTBB images/snowflake.jpg audio/snowflakeTBB10.wav 10
# ./CONVERTTBBISPC images/snowflake.jpg audio/snowflakeTBBISPC10.wav 10

# echo "Image: elephant(1517 × 1144) with time step 10"
# ./CONVERT images/elephant.jpg audio/elephantSerial10.wav 10 
# ./CONVERTTBB images/elephant.jpg audio/elephantTBB10.wav 10 
# ./CONVERTTBBISPC images/elephant.jpg audio/elephantTBBISPC10.wav 10

# echo "Image: crosses(2000 × 1414) with time step 10"
# ./CONVERT images/crosses.jpg audio/crossesSerial10.wav 10 
# ./CONVERTTBB images/crosses.jpg audio/crossesTBB10.wav 10 
# ./CONVERTTBBISPC images/crosses.jpg audio/crossesTBBISPC10.wav 10

# echo "Image: zebra(7016 × 9933) with time step 10"
# ./CONVERT images/zebra.jpg audio/zebraSerial10.wav 10 
# ./CONVERTTBB images/zebra.jpg audio/zebraTBB10.wav 10 
# ./CONVERTTBBISPC images/zebra.jpg audio/zebraTBBISPC10.wav 10

# echo "Image: stars(9600 × 7200) with time step 10"
# ./CONVERT images/stars.jpg audio/starsSerial10.wav 10 
# ./CONVERTTBB images/stars.jpg audio/starsTBB10.wav 10 
# ./CONVERTTBBISPC images/stars.jpg audio/starsTBBISPC10.wav 10

# echo "Image: shoe(9648 × 7236) with time step 10"
# ./CONVERT images/shoe.jpeg audio/shoeSerial10.wav 10 
# ./CONVERTTBB images/shoe.jpeg audio/shoeTBB10.wav 10 
# ./CONVERTTBBISPC images/shoe.jpeg audio/shoeTBBISPC10.wav 10

# echo "Image: fractal(11520 × 6262) with time step 10"
# ./CONVERT images/fractal.jpg audio/fractalSerial10.wav 10 
# ./CONVERTTBB images/fractal.jpg audio/fractalTBB10.wav 10 
# ./CONVERTTBBISPC images/fractal.jpg audio/fractalTBBISPC10.wav 10

# echo "Image: space(12000 × 6000) with time step 10"
# ./CONVERT images/space.jpg audio/spaceSerial10.wav 10 
# ./CONVERTTBB images/space.jpg audio/spaceTBB10.wav 10 
# ./CONVERTTBBISPC images/space.jpg audio/spaceTBBISPC10.wav 10




#Unsure

#echo "Image: city(15758 × 5528) with time step 10"
# ./CONVERT images/city.jpeg audio/citySerial10.wav 10 
# ./CONVERTTBB images/city.jpeg audio/cityTBB10.wav 10 
#./CONVERTTBBISPC images/city.jpeg audio/cityTBBISPC10.wav 10

# echo "Image: planets(23520 × 4320) with time step 10"
# ./CONVERT images/planets.jpg audio/planetsSerial10.wav 10 
# ./CONVERTTBB images/planets.jpg audio/planetsTBB10.wav 10 
# ./CONVERTTBBISPC images/planets.jpg audio/planetsTBBISPC10.wav 10