EC2_TOKEN=$(curl -X PUT "http://169.254.169.254/latest/api/token" -H "X-aws-ec2-metadata-token-ttl-seconds: 21600" 2>/dev/null || wget -q -O - --method PUT "http://169.254.169.254/latest/api/token" --header "X-aws-ec2-metadata-token-ttl-seconds: 21600" 2>/dev/null)
HEADER="X-aws-ec2-metadata-token: $EC2_TOKEN"
URL="http://169.254.169.254/latest/meta-data"

aws_req=""
if [ "$(command -v curl)" ]; then
    aws_req="curl -s -f -H '$HEADER'"
elif [ "$(command -v wget)" ]; then
    aws_req="wget -q -O - -H '$HEADER'"
else
    echo "Neither curl nor wget were found, I can't enumerate the metadata service :("
fi

printf "ami-id: "; eval $aws_req "$URL/ami-id"; echo ""
printf "instance-action: "; eval $aws_req "$URL/instance-action"; echo ""
printf "instance-id: "; eval $aws_req "$URL/instance-id"; echo ""
printf "instance-life-cycle: "; eval $aws_req "$URL/instance-life-cycle"; echo ""
printf "instance-type: "; eval $aws_req "$URL/instance-type"; echo ""
printf "region: "; eval $aws_req "$URL/placement/region"; echo ""

echo ""
echo "Account Info"
eval $aws_req "$URL/identity-credentials/ec2/info"; echo ""
eval $aws_req "http://169.254.169.254/latest/dynamic/instance-identity/document"; echo ""

echo ""
echo "Network Info"
for mac in $(eval $aws_req "$URL/network/interfaces/macs/" 2>/dev/null); do
  echo "Mac: $mac"
  printf "Owner ID: "; eval $aws_req "$URL/network/interfaces/macs/$mac/owner-id"; echo ""
  printf "Public Hostname: "; eval $aws_req "$URL/network/interfaces/macs/$mac/public-hostname"; echo ""
  printf "Security Groups: "; eval $aws_req "$URL/network/interfaces/macs/$mac/security-groups"; echo ""
  echo "Private IPv4s:"; eval $aws_req "$URL/network/interfaces/macs/$mac/ipv4-associations/"; echo ""
  printf "Subnet IPv4: "; eval $aws_req "$URL/network/interfaces/macs/$mac/subnet-ipv4-cidr-block"; echo ""
  echo "PrivateIPv6s:"; eval $aws_req "$URL/network/interfaces/macs/$mac/ipv6s"; echo ""
  printf "Subnet IPv6: "; eval $aws_req "$URL/network/interfaces/macs/$mac/subnet-ipv6-cidr-blocks"; echo ""
  echo "Public IPv4s:"; eval $aws_req "$URL/network/interfaces/macs/$mac/public-ipv4s"; echo ""
  echo ""
done

echo ""
echo "IAM Role"
eval $aws_req "$URL/iam/info"
for role in $(eval $aws_req "$URL/iam/security-credentials/" 2>/dev/null); do
  echo "Role: $role"
  eval $aws_req "$URL/iam/security-credentials/$role"; echo ""
  echo ""
done

echo ""
echo "User Data"
# Search hardcoded credentials
eval $aws_req "http://169.254.169.254/latest/user-data"

echo ""
echo "EC2 Security Credentials"
eval $aws_req "$URL/identity-credentials/ec2/security-credentials/ec2-instance"; echo ""
