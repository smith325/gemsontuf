from tuf.libtuf import *
import os

#key generate, they're all the same
generate_and_write_rsa_keypair("root_key", bits=2048, password="password")
generate_and_write_rsa_keypair("targets_key", bits=2048, password="password")
generate_and_write_rsa_keypair("release_key", bits=2048, password="password")
generate_and_write_rsa_keypair("timestamp_key", bits=2048, password="password")

#public key import
publicRoot = import_rsa_publickey_from_file("root_key.pub")
publicTarget = import_rsa_publickey_from_file("targets_key.pub")
publicRelease = import_rsa_publickey_from_file("release_key.pub")
publicTime = import_rsa_publickey_from_file("timestamp_key.pub")

#private key import second parameter is so you don't have to type a password in
privateRoot = import_rsa_privatekey_from_file("root_key", password="password")
privateTarget = import_rsa_privatekey_from_file("targets_key", password="password")
privateRelease = import_rsa_privatekey_from_file("release_key", password="password")
privateTime = import_rsa_privatekey_from_file("timestamp_key", password="password")

#create new repository directory
repository = create_new_repository("repository")

#adds public keys to directory
repository.root.add_key(publicRoot)
repository.targets.add_key(publicTarget)
repository.release.add_key(publicRelease)
repository.timestamp.add_key(publicTime)

#expiration date
repository.timestamp.expiration = "2014-10-28 12:08:00"

#creates a symlink to the repository and then creates the target metadata 
#adds targets metadata
os.symlink("../../repository/targets", "./repository/targets/test")
targetFiles = repository.get_filepaths_in_directory("./repository/targets/test",
                                                        recursive_walk=True, followlinks=True)
repository.targets.add_targets(targetFiles)

#creates threshold for root and loads private keys
repository.root.threshold = 1
repository.root.load_signing_key(privateRoot)
repository.targets.load_signing_key(privateTarget)
repository.release.load_signing_key(privateRelease)
repository.timestamp.load_signing_key(privateTime)

#prints some information about the repository setup
repository.status()

#tries to create repository 
try:
  repository.write()
except tuf.Error, e:
  print e 
