def get_value_if_prefix_matches(text, prefix):
    if text.startswith(prefix):
        return text[len(prefix):]
    return None
