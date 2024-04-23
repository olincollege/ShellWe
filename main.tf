provider "aws" {
  region  = var.region
}

# Create a vpc
resource "aws_vpc" "chat_vpc" {
  cidr_block           = "10.0.0.0/16"
  enable_dns_support   = true
  enable_dns_hostnames = true
  tags                 = {
    name = "ChatVPC"
  }
}

# Create an internet gateway
resource "aws_internet_gateway" "chat_gw" {
  vpc_id = aws_vpc.chat_vpc.id
  tags   = {
    name = "chat_gw"
  }
}

# Create a custom route table
resource "aws_route_table" "chat_route_table" {
  vpc_id = aws_vpc.chat_vpc.id
  tags   = {
    name = "my_route_table"
  }
}
# create route
resource "aws_route" "chat_route" {
  destination_cidr_block = "0.0.0.0/0"
  gateway_id             = aws_internet_gateway.chat_gw.id
  route_table_id         = aws_route_table.chat_route_table.id
}

# create a subnet
resource "aws_subnet" "chat_subnet" {
  vpc_id            = aws_vpc.chat_vpc.id
  cidr_block        = "10.0.1.0/24"
  availability_zone = var.availability_zone

  tags = {
    name = "chat_subnet"
  }
}

# associate internet gateway to the route table by using subnet
resource "aws_route_table_association" "chat_assoc" {
  subnet_id      = aws_subnet.chat_subnet.id
  route_table_id = aws_route_table.chat_route_table.id
}

# create security group to allow ingoing ports
resource "aws_security_group" "chat_sg" {
  name        = "chat_server_sg"
  description = "security group for the EC2 instance of chat server"
  vpc_id      = aws_vpc.chat_vpc.id
  ingress     = [
    {
      description      = "ssh"
      from_port        = 22
      to_port          = 22
      protocol         = "tcp"
      cidr_blocks      = ["0.0.0.0/0", aws_vpc.chat_vpc.cidr_block]
      ipv6_cidr_blocks = []
      prefix_list_ids  = []
      security_groups  = []
      self             = false
    },
    {
      description      = "TCP access for chat server"
      from_port        = 4000
      to_port          = 65535
      protocol         = "tcp"
      cidr_blocks      = ["0.0.0.0/0", aws_vpc.chat_vpc.cidr_block]
      ipv6_cidr_blocks = []
      prefix_list_ids  = []
      security_groups  = []
      self             = false
    },
  ]
  egress = [
    {
      from_port        = 0
      to_port          = 0
      protocol         = "-1"
      cidr_blocks      = ["0.0.0.0/0"]
      description      = "Outbound traffic rule"
      ipv6_cidr_blocks = []
      prefix_list_ids  = []
      security_groups  = []
      self             = false
    }
  ]
  tags = {
    name = "allow_ssh_and_chat_server"
  }
}

# create a network interface with private ip from step 4
resource "aws_network_interface" "chat_net_interface" {
  subnet_id       = aws_subnet.chat_subnet.id
  security_groups = [aws_security_group.chat_sg.id]
}

# assign a elastic ip to the network interface created in step 7
resource "aws_eip" "chat_eip" {
  domain                    = "vpc"
  network_interface         = aws_network_interface.chat_net_interface.id
  associate_with_private_ip = aws_network_interface.chat_net_interface.private_ip
  depends_on                = [
    aws_internet_gateway.chat_gw, aws_instance.chat_server
  ]
}

# Launch an EC2 instance in the subnet with the defined security group.
resource "aws_instance" "chat_server" {
  ami               = "ami-0a125f1b5cba564a0"
  instance_type     = var.instance_type
  availability_zone = var.availability_zone

  key_name = "aws_key"

  network_interface {
    device_index         = 0
    network_interface_id = aws_network_interface.chat_net_interface.id
  }
  user_data = file("${path.module}/setup_server.sh")
  tags      = {
    Name = "chat_server"
  }
}

output "instance_public_ip" {
  description = "Public IP address of the EC2 instance"
  value       = aws_instance.chat_server.private_ip
}

output "eip" {
  description = "public Ip of eip"
  value       = aws_eip.chat_eip.public_ip
}

output "subnet_id" {
  description = "private ip(subnet)"
  value       = aws_subnet.chat_subnet.id
}
