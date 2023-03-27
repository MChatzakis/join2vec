# Info https://www.cs.sfu.ca/~jnwang/projects/adapt/
LINK=https://www.cs.sfu.ca/~jnwang/codes/adapt.tar.gz
NAME=adapt.tar.gz
FNAME=adapt

cd ../bin

wget $LINK
tar -xvf $NAME

cp $FNAME/linux/AdaptJoin ./

rm -rf ./adapt
rm adapt.tar.gz	adapt.tar.gz.*

cd ../scripts