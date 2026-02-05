## Benefits of Cloud Computing

--- 

- **Date:** 2026-01-31
- **Tags:** [Aws](tags/Aws.md)
- **URL:**  

---

### Summary

# Benefits of Cloud Computing

Cloud computing is the technology that allows for the delivery of computing resources (such as servers, storage, databases, networking, software, analytics, and intelligence) over the internet ("the cloud") to offer faster innovation,flexible resources, and economies of scale. Here are some of the benefits of Cloud Computing:

* On-demand Self-service: Users can provision computing resources automatically, without requiring human interaction with each service provider.

* Broad Network Access: Cloud services are available over the network and accessed through standard mechanisms, promoting use by heterogeneous client platforms (e.g., mobile phones, tablets, laptops, and workstations).

* Resource Pooling: The provider’s computing resources are pooled to serve multiple consumers using a multi-tenant model, with different physical and virtual resources dynamically assigned and reassigned according to consumer demand.

* Rapid Elasticity: Capabilities can be elastically provisioned and released, in some cases automatically, to scale rapidly outward and inward commensurate with demand.

* Measured Service: Cloud systems automatically control and optimize resource use by leveraging a metering capability at some level of abstraction appropriate to the type of service (e.g., storage, processing, bandwidth), so resource usage can be monitored, controlled, and reported.

# Cloud Service Models

* Software as a Service (SaaS):It gives the organization the complete solution, including the hardware, as well as the OS, the middleware, the runtime, the data processing, and the application or software thats needed for the service to be delivered to your end user. Example: AWS offers SaaS solutions like Amazon Chime (video conferencing) and AWS Marketplace SaaS products. 

* Platform as a Service (PaaS): The service provider will provide only the hardware, including the networking, storage, and servers. The user will have to manage the applications and data. Example: AWS Elastic Beanstalk allows developers to deploy and manage applications without managing the underlying infrastructure.

* Infrastructure as a Service (IaaS): The service provider is going to give the organization the hardware that is needed, including networking, storage, and servers. But no software. Example: AWS EC2 provides virtual servers (infrastructure) that users can configure and manage as needed.

* Function as a Service (FaaS): It is a serverless computing service that allows users to run code in response to events without the complexity of building and maintaining the infrastructure typically associated with developing and launching an app. Example: AWS Lambda lets you run code in response to S3 uploads or API Gateway requests without provisioning servers.

# Cloud Deployment Models

* Public Cloud: The cloud infrastructure is made available to the general public or a large industry group and is owned by an organization selling cloud services. Example: Hosting a public-facing website on AWS S3 and CloudFront is a public cloud use case.

* Private Cloud: Its a cloud dedicated to a single organization, and its infrastructure, resources and data are all exclusively used by that organization. Aws offers a service called Virtual Private Cloud(VPC) that allows users to create a private cloud within the public cloud.

* Hybrid Cloud: It is a combination of two or more clouds (private, public, or community) that remain unique entities but are bound together by standardized or proprietary technology that enables data and application portability between them (e.g., hybrid cloud bursting for load balancing between clouds). 

* Community Cloud: The cloud infrastructure is shared by several organizations and supports a specific community that has shared concerns (e.g., mission, security requirements, policy, and compliance considerations). It may be managed by the organizations or a third party and may exist on or off premises. Example: AWS GovCloud provides a community cloud for US government agencies and contractors with specific compliance requirements.

* Multi-Tenancy Model: In this model, multiple customers (tenants) share the same computing resources (like servers and storage) provided by a cloud service provider. Each tenant's data is isolated and remains invisible to other tenants, ensuring privacy and security. This model is cost-effective as it allows for resource sharing, leading to reduced costs for each tenant. Example: AWS S3 stores data for millions of customers on shared infrastructure, but each customer's data is logically isolated.

* Single-Tenancy Model: In this model, each customer (tenant) has their own dedicated computing resources. This means that the hardware and software are not shared with other tenants. This model can offer enhanced security and performance since resources are not shared, but it is typically more expensive than multi-tenancy due to the dedicated nature of the resources. Example: AWS Dedicated Hosts provide physical servers dedicated to a single customer for compliance or licensing needs.

# Shared Responsibility Model

* Cloud Provider Responsibilities: The cloud provider is responsible for the security "of" the cloud. This includes the physical security of the data centers, the hardware, and the software that runs the cloud services. For example, AWS is responsible for securing the infrastructure that runs all of the services offered in the AWS Cloud.
* Customer Responsibilities: The customer is responsible for security "in" the cloud. This includes managing and securing their data, applications, and operating systems. For example, if a customer is using AWS EC2 instances, they are responsible for securing the operating system, applications, and data on those instances.
