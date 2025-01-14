import random

def generate_personalized_email(target_info):
    greetings = ["Dear {name},", "Hi {name},", "Hello {name},"]
    intros = [
        "I hope this email finds you well.",
        "I hope you are having a great day.",
        "I trust you are doing well."
    ]
    project_related_phrases = [
        "Regarding the {project} project that you are currently working on,",
        "Concerning the {project} project,",
        "About the {project} project,"
    ]

    interest_related_phrases = [
        "Given your interest in {interest},",
        "Considering your passion for {interest},",
        "I noticed that you are interested in {interest},"
    ]

    colleague_mentions = [
        "Your colleague, {colleague}, suggested I reach out to you.",
        "I was recently talking with {colleague} and they recommended that I contact you."
    ]

    closings = ["Best regards,", "Sincerely,", "Regards,"]

    email_content = random.choice(greetings).format(name=target_info['name']) + "\n\n"
    email_content += random.choice(intros) + "\n\n"
    email_content += random.choice(project_related_phrases).format(project=target_info['project']) + "\n"
    email_content +=  "I came across this interesting article: [malicious_link]\n\n"
    email_content += random.choice(interest_related_phrases).format(interest=target_info['interest']) + "\n\n"
    email_content += random.choice(colleague_mentions).format(colleague=target_info['colleague']) + "\n"
    email_content +=  "Please review at your convenience.\n\n"
    email_content += random.choice(closings) + "\n\n"
    email_content += "[Fake Sender Name]"

    return email_content

target = {
  "name" : "John Smith",
  "project" : "X Project",
  "interest" : "cybersecurity",
  "colleague" : "Mary"
}
print(generate_personalized_email(target))
