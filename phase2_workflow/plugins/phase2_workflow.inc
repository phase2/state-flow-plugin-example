<?php

class Phase2Workflow extends StateFlow {
  public function init() {
    $this->create_state('draft');
    $this->create_state('needs_review');
    $this->create_state('approved', array(
      'on_enter' => array($this, 'on_enter_approved'),
    ));
    $this->create_state('published', array(
      'on_enter' => array($this, 'on_enter_published'),
    ));
    $this->create_state('unpublished', array(
      'on_exit' => array($this, 'on_enter_unpublished'),
    );

    $this->create_event('submit_for_review', array(
      'label' => t('Submit for review'),
      'origin' => 'draft',
      'target' => 'needs_review',
    ));
    $this->create_event('approve', array(
      'label' => t('Approve'),
      'origin' => 'needs_review',
      'target' => 'approved',
      'guard' => 'phase2_workflow_guard_approve',
    ));
    $this->create_event('request_updates', array(
      'label' => t('Request Updates'),
      'origin' => array('published', 'approved', 'needs_review'),
      'target' => 'draft',
    ));
    $this->create_event('publish', array(
      'label' => t('Publish'),
      'origin' => 'approved',
      'target' => 'published',
      'guard' => 'phase2_workflow_guard_publish',
    ));
    $this->create_event('unpublish', array(
      'label' => t('Unpublish'),
      'origin' => 'published',
      'target' => 'unpublished',
      'guard' => 'phase2_workflow_guard_publish',
    ));
  }

  public function on_enter_published() {
    $node = $this->get_object();
    $node->status = 1;
    node_save($node);
  }

  public function on_enter_unpublished() {
    $node = $this->get_object();
    $node->status = 0;
    node_save($node);
  }

  public function on_enter_approved() {
    $node = $this->get_object();
    // Do some custom code to fulfill your requirement.
    // Perhaps alert the author or some stakeholders
  }
}