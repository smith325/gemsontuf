from tuf.libtuf import *

#key generate
generate_and_write_rsa_keypair("root_key", bits=2048, password="password")
generate_and_write_rsa_keypair("targets_key", password="password")
generate_and_write_rsa_keypair("release_key", password="password")
generate_and_write_rsa_keypair("timestamp_key", password="password")


#public key import
publicRoot = import_rsa_publickey_from_file("root_key.pub")
publicTarget = import_rsa_publickey_from_file("targets_key.pub")
publicRelease = import_rsa_publickey_from_file("release_key.pub")
publicTime = import_rsa_publickey_from_file("timestamp_key.pub")

#private key import
privateRoot = import_rsa_privatekey_from_file("root_key", password="password")
privateTarget = import_rsa_privatekey_from_file("targets_key", password="password")
privateRelease = import_rsa_privatekey_from_file("release_key", password="password")
privateTime = import_rsa_privatekey_from_file("timestamp_key", password="password")

repository = create_new_repository("repository")

repository.root.add_key(publicRoot)
repository.targets.add_key(publicTarget)
repository.release.add_key(publicRelease)
repository.timestamp.add_key(publicTime)


repository.root.threshold = 1
repository.root.load_signing_key(privateRoot)
repository.targets.load_signing_key(privateTarget)
repository.release.load_signing_key(privateRelease)
repository.timestamp.load_signing_key(privateTime)

repository.timestamp.expiration = "2014-10-28 12:08:00"



repository.status()


try:
  repository.write()
except tuf.Error, e:
  print e 
