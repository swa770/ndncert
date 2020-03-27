# NDN Certificate Management Protocol (NDNCERT)

[![Build Status](https://travis-ci.org/named-data/ndncert.svg?branch=master)](https://travis-ci.org/named-data/ndncert)

NDN certificate management protocol (NDNCERT) enables automatic certificate management in
NDN. In Named Data Networking (NDN), every entity should have corresponding identity
(namespace) and the corresponding certificate for this namespace. Moreover, entities need
simple mechanisms to manage sub-identities and their certificates. NDNCERT provides flexible
mechanisms to request certificate from a certificate authority(CA) and, as soon as certificate
is obtained, mechanisms to issue and manage certificates in the designated namespace. Note that
NDNCERT does not impose any specific trust model or trust anchors.  While the primary use case
of the developed protocol is to manage NDN testbed certificates, it can be used with any other
set of global and local trust anchors.

This specification provides details and packet formats to request certificates, create
certificates after one of the validation mechanism, and how the issued certificate is retrieved
by the original requester.

See [our GitHub wiki](https://github.com/named-data/ndncert/wiki/NDNCERT-Protocol-0.2) for more details.

## 1. Build and Installation Instructions

### 1.1 Build from source code

#### Prerequisites

* Install the [ndn-cxx](https://named-data.net/doc/ndn-cxx/current/) library and its prerequisites.
  Please see [Getting Started with ndn-cxx](https://named-data.net/doc/ndn-cxx/current/INSTALL.html)
  for how to install ndn-cxx.
  Note: If you have installed ndn-cxx from a binary package, please make sure development headers
  are installed (if using Ubuntu PPA, `libndn-cxx-dev` package is needed).

  Any operating system and compiler supported by ndn-cxx is supported by NDNCERT.

#### Build Steps

To configure, compile, and install NDNCERT, type the following commands
in ndn-tools source directory:

```bash
./waf configure
./waf
sudo ./waf install
```

To uninstall NDNCERT:

```bash
sudo ./waf uninstall
```

### 1.2 Install NDNCERT Using the NDN PPA Repository on Ubuntu Linux

NFD binaries and related tools for the latest versions of Ubuntu Linux can be installed using PPA packages from named-data repository. First, you will need to add named-data/ppa repository to binary package sources and update list of available packages.

#### Preliminary steps if you have not used PPA packages before

To simplify adding new PPA repositories, Ubuntu provides `add-apt-repository` tool,
which is not installed by default on some systems.

```bash
sudo apt-get install software-properties-common
```

#### Adding NDN PPA

After installing `add-apt-repository`, run the following command to add `NDN PPA repository`.

```bash
sudo add-apt-repository ppa:named-data/ppa
sudo apt-get update
```

#### Installing NDNCERT (Don't use this for now because we haven't tested the PPA yet)

After you have added `NDN PPA repository`, NFD and other NDN packages can be easily
installed in a standard way, i.e., either using `apt-get` as shown below or using any
other package manager, such as Synaptic Package Manager:

```bash
sudo apt-get install ndncert
```

## 2. A Quick Start

### 2.1 Use NDNCERT as a client

#### 2.1.1 Set up client configuration

```bash
mv /usr/local/etc/ndncert/client.conf.sample /usr/local/etc/ndncert/client.conf
```

Then edit `client.conf` to correctly contain CAs that you want to communicate with.
CA information should be obtained through a trustworthy channel.
For how to edit `client.conf`, see [NDNCERT Client Configuration Spec](https://github.com/named-data/ndncert/wiki/NDNCERT-Client-Configuration) for more details.

Another way of installing config file is to use NDNCERT's INFO function.
To do so, in the interactive command line tool ndncert-client:

```ascii
Step 0: Please type in the CA INDEX that you want to apply or type in NONE if your expected CA is not in the list
```

Type in `NONE` to trigger INFO function.

```ascii
Step 1: Please type in the CA Name
```

Type in the CA's NDN identity name, e.g., `/ndn/edu/ucla/ndncert`, and ndncert-client will fetch the CA's profile back.
The CA's information along with its public key bits will be shown later and user **must** verify the information is correct (i.e., the CA is really the one that you want to communicate to) before moving to later steps.

#### 2.1.2 Run NDNCERT Client

Use the following interactive command line tool to become a NDNCERT client.

```bash
ndncert-client
```

### 2.2 Use NDNCERT as a CA

#### 2.2.1 Set up CA configuration

```bash
mv /usr/local/etc/ndncert/ca.conf.sample /usr/local/etc/ndncert/ca.conf
```

Then edit `ca.conf` to correctly reflect your settings of your CA.
An example can be as follow:

```bash
{
  "ca-prefix": "/ndn/edu/ucla/ndncert",
  "issuing-freshness": "720",
  "validity-period": "360",
  "probe": "email",
  "ca-info": "NDN Testbed CA",
  "supported-challenges":
  [
      { "type": "PIN" },
      { "type": "Email" }
  ]
}
```

The most important attribute is `ca-prefix`, which refers to a identity name in your local NDN key chain.
This means you must already have the key generated in your key chain.

For how to edit `ca.conf`, see [NDNCERT CA Configuration Spec](https://github.com/named-data/ndncert/wiki/NDNCERT-CA-Configuration) for more details.

#### 2.2.2 Use NDNCERT as a CA with systemd and NFD PPA

The other way is to run NDNCERT CA with System Service if you are using Ubuntu linux.
In this case, the key should be kept in user `ndn`'s NDN key chain located in a newly create directory `/var/lib/ndn/ndncert-ca`
To manage the key of user `ndn`, you can add a prefix `sudo HOME=/var/lib/ndn/ndncert-ca -u ndn` before your command.

For example, to list/generate your keys, use:

```bash
sudo HOME=/var/lib/ndn/ndncert-ca -u ndn ndnsec-ls-identity -k
sudo HOME=/var/lib/ndn/ndncert-ca -u ndn  ndnsec-keygen /example
```

To set up your NDNCERT CA:

##### First copy paste the systemd file into the system directory

```bash
mkdir /var/lib/ndn/ndncert-ca
chown ndn /var/lib/ndn/ndncert-ca
sudo cp /path/to/NDNCERT/systemd/ndncert.service /etc/systemd/system
sudo chmod 644 /etc/systemd/system/ndncert.service
```

##### Start, stop, restart, and monitor the service

```bash
sudo systemctl start ndncert
sudo systemctl stop ndncert
sudo systemctl restart ndncert
sudo systemctl status ndncert
```

##### Check the log of NDNCERT CA

```bash
sudo journalctl -u ndncert
```

#### 2.2.3 Use NDNCERT as a CA with the command line tool

One way to start a NDNCERT CA daemon is to use NDNCERT Command Line Tool.

```bash
ndncert-ca-server
```
